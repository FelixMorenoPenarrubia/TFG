#include "Graph.hh"
#include "debug_utility.hh"
#include<algorithm>

using std::vector;
using std::map;

Graph::Graph() {
    n = 0;
    m = 0;
    al = vector<vector<int>>();
    ral = vector<map<int, int>>();
}

Graph::Graph(vector<vector<int>> _al)  {
    al = _al;
    n = _al.size();
    generate_ral_and_m();
}

bool Graph::empty() const {
    return n == 0;
}

bool Graph::neighbors(int u, int v) const {
    return ral[u].find(v) != ral[u].end();
}

void Graph::generate_ral_and_m() {
    ral = vector<std::map<int, int>>(n);
    for (int u=0; u < n; ++u) {
        for (int j=0; j < (int)al[u].size(); ++j) {
            int v = al[u][j];
            ral[u][v] = j;
            m++;
        }
    }
    m /= 2;
}

void Graph::set_starting_edge_in_al(int u, int v) {
    vector<int> newal;
    int oldidx = ral[u][v];
    int als = al[u].size();
    for (int i=0; i < als; ++i) {
        newal.push_back(al[u][(i+oldidx)%als]);
        ral[u][al[u][(i+oldidx)%als]] = i;
    }
    al[u] = newal;
    m = 0;
    generate_ral_and_m();
}

void Graph::set_outer_face(vector<int> of) {
    int ofs = of.size();
    for (int i=0; i < ofs; ++i) {
        set_starting_edge_in_al(of[i], of[(i+1)%ofs]);
    }
}

vector<int> Graph::get_face_ccw(int u, int v) const {
    debug_assert(neighbors(u, v));

    vector<int> ans;
    ans.push_back(u);
    int nxt = v;
    int prev = u;
    while (nxt != u && (int)ans.size() < n+1) {
        ans.push_back(nxt);
        int tmp = al[nxt][(ral[nxt].at(prev)+1)%al[nxt].size()];
        prev = nxt;
        nxt = tmp;
    }
    if ((int)ans.size() == n+1) return vector<int>();
    return ans;
}

Graph Graph::identify_vertices(vector<int> v1,  vector<int> v2) const {
    vector<int> morph(n);
    vector<int> corrv1(n, -1);
    int pen = 0;

    for (int j=0; j < (int)v1.size(); ++j) {
        corrv1[v2[j]] = v1[j];
    }
    for (int i=0; i < n; ++i) {
        morph[i] = i-pen;
        if (corrv1[i] != -1) pen++;
    }
    for (int j=0; j < (int)v1.size(); ++j) {
        morph[v2[j]] = morph[v1[j]];
    }

    Graph g;
    g.al = vector<vector<int>>(n-(int)v1.size());

    for (int u=0; u < n; ++u) {
        for (int v : al[u]) {
            if (corrv1[v] != -1) {
                if (ral[u].find(corrv1[v]) != ral[u].end() || (corrv1[u] != -1 && ral[corrv1[u]].find(corrv1[v]) != ral[corrv1[u]].end())) {
                    continue;
                }
            }
            if (corrv1[u] != -1) {
                if (ral[corrv1[u]].find(v) != ral[corrv1[u]].end()) {
                    continue;
                }
            }
            g.al[morph[u]].push_back(morph[v]);
        }
    }
    g.generate_ral_and_m();
    return g;
}

bool Graph::subgraph_isomorphism(const Graph& h) const {
    vector<int> f (h.n, -1); //to which vertex from g are the vertices of h mapped to
    vector<int> used(n, 0); // which vertices are used
    int cv = 0; //current vertex of h that we are mapping

    // test three properties:
    // 1. vertex not already used
    //
    // 3. neighbors from previously selected vertices
    auto test_vertex = [&] () -> bool {
        if (used[f[cv]]) return false;

        for (int cu = 0; cu < cv; ++cu) {
            if (!neighbors(f[cu], f[cv]) && h.neighbors(cu, cv)) return false;
        }

        return true;
    };
    while (cv != -1) {
        if (cv == h.n) {
            return true;
        }
        if (f[cv] > -1) {
            used[f[cv]] = 0;
        }
        f[cv]++;
        while (f[cv] < n && !test_vertex()) {
            f[cv]++;
        }
        if (f[cv] == n) {
            f[cv] = -1;
            cv--;
        }
        else {
            used[f[cv]] = 1;
            cv++;
        }

    }
    return false;
}

void Graph::write_cpp(std::ostream& os) const {
    os << n << " " << m << std::endl;
    for (int u=0; u < n; ++u) {
        os << "{";
        for (int j=0; j < (int)al[u].size(); ++j) {
            if (j > 0) os << ", ";
            os << al[u][j];
        }
        os << "}";
        if (u+1 < n) os << ",";
        os << std::endl;
    }
}

bool Graph::connected() const {
    unsigned long long vis_bm = 1;
    vector<int> st;
    st.push_back(0);
    while (!st.empty()) {
        int u = st.back();
        st.pop_back();
        for (int v : al[u]) {
            if (!(vis_bm&(1LL<<v))) {
                vis_bm |= (1LL<<v);
                st.push_back(v);
            }
        }
    }
    return vis_bm+1 == (1ULL<<n);
}
