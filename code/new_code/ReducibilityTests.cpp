#include "ReducibilityTests.hh"
#include "AlonTarsi.hh"
#include "debug_utility.hh"
#include<functional>

using std::vector;
using std::map;

bool degree_test(const ListGraph& g) {
    for (int u = 0; u < g.n; ++u) {
        if ((int)g.al[u].size() < g.list_sizes[u]) return true;
    }
    return false;
}

bool alon_tarsi_test(const ListGraph& g) {
    return alon_tarsi(g);
}

bool batch_test(const ListGraph& g) {
    vector<std::function<bool(const ListGraph&)>> tests = {degree_test, alon_tarsi_test};

    for (auto f : tests) {
        if (f(g)) return true;
    }
    return false;
}

vector<int> minimal_irreducible_deletedvertices(const ListGraph& g) {
    for (int v = 0; v < g.n; ++v) {
        ListGraph gp = g.remove_vertex(v);
        if(!batch_test(gp)) {
            vector<int> dv = minimal_irreducible_deletedvertices(gp);
            for (int i=0; i < (int)dv.size(); ++i) {
                if(dv[i] >= v) dv[i]++;
            }
            dv.push_back(v);
            return dv;
        }
    }
    return vector<int>();
}

bool recursive_reducibility_batch_test(const ListGraph& g) {
    static map<ListGraphCode, bool> mem;

    debug_msg("recursive_redu_b_t");
    debug_var(g.n);
    debug_var(g.m);
    debug_msg(g.compute_list_code().to_string());

    if (g.empty()) return false;

    vector<ListGraph> vcc = g.connected_components();

    if (vcc.size() > 1) {
        for (ListGraph gi : vcc) {
            if (recursive_reducibility_batch_test(gi)) return true;
        }
        return false;
    }

    ListGraphCode code = g.compute_list_code();

    if (mem.find(code) != mem.end()) {
        return mem[code];
    }

    if (g.nocolors()) return mem[code] = false;

    for (int u = 0; u < g.n; ++u) {
        if (g.list_sizes[u] <= 0) {
            return mem[code] = recursive_reducibility_batch_test(g.precolor_vertex(u));
        }
    }

    if (batch_test(g)) {
        return mem[code] = true;
    }
    vector<int> deleted_vertices = minimal_irreducible_deletedvertices(g);

    vector<int> deleted(g.n);
    for (int v : deleted_vertices) deleted[v] = 1;
    int undel_vertex = 0;
    while (deleted[undel_vertex]) undel_vertex++;
    ListGraph ng = g.precolor_vertex(undel_vertex);
    return mem[code] = recursive_reducibility_batch_test(ng);
}