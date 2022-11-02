#include "ReducibilityTests.hh"
#include "DFSGraph.hh"
#include "AlonTarsi.hh"
#include "debug_utility.hh"
#include<functional>

using std::vector;
using std::map;

bool degree_test(const ListGraph& g) {
    for (int u = 0; u < g.n; ++u) {
        if ((int)g.al[u].size() < g.list_sizes[u]) {
             return true;
        }
    }
    return false;
}

bool biconnected_components_degreeassignment_test(const ListGraph& g) {

    //debug_msg("Biconnected components test");

    vector<DFSGraph> comp = DFSGraph(g.degree_assignment_subgraph()).partition_biconnected();

    //debug_var(comp.size());

    for (DFSGraph df : comp) {
        //debug_var(df.n);
        //debug_var(df.al);
        if (!(df.is_odd_cycle() || df.is_clique())) return true;
    }
    return false;
}

//TODO: generalize gadget tests
bool gadget4_test(const ListGraph& g) {
    for (int u = 0; u < g.n; ++u) {
			if ((g.list_sizes[u]-(int)g.al[u].size()+3) >= 3) {
				for (int v : g.al[u]) {
					if ((g.list_sizes[v]-(int)g.al[v].size()+2) >= 2) {
						for (int w : g.al[u]) {
							if ((g.list_sizes[w]-(int)g.al[w].size()+3) >= 2 && g.neighbors(v, w)) {
								for (int x : g.al[u]) {
									if ((g.list_sizes[x]-(int)g.al[x].size()+2) >= 2 && g.neighbors(x, w) && x != v ) {
										return true;
									}
								}
							}
						}
					}
				}
			}
		}
		return false;
}

bool gadget5_test(const ListGraph& g) {
    for (int u = 0; u < g.n; ++u) {
			if ((g.list_sizes[u]-(int)g.al[u].size()+4) >= 4) {
				for (int v : g.al[u]) {
					if ((g.list_sizes[v]-(int)g.al[v].size()+2) >= 2) {
						for (int w : g.al[u]) {
							if ((g.list_sizes[w]-(int)g.al[w].size()+3) >= 2 && g.neighbors(v, w)) {
								for (int x : g.al[u]) {
									if ((g.list_sizes[x]-(int)g.al[x].size()+3) >= 2 && g.neighbors(x, w) && x != v ) {
                                        for (int y : g.al[u]) {
                                            if ((g.list_sizes[y]-(int)g.al[y].size()+2) >= 2 && g.neighbors(x, y) && y != w && y != v) {
										        return true;
                                            }
                                        }
									}
								}
							}
						}
					}
				}
			}
		}
		return false;
}



bool alon_tarsi_test(const ListGraph& g) {
    static map<ListGraphCode, bool> mem;
    ListGraphCode code = g.compute_list_code();
    if (mem.find(code) == mem.end()) mem[code] = alon_tarsi(g);
    return mem[code];
}

bool batch_reducible_test(const ListGraph& g) {
    vector<std::function<bool(const ListGraph&)>> tests = {degree_test, biconnected_components_degreeassignment_test, gadget4_test, gadget5_test, alon_tarsi_test};

    for (auto f : tests) {
        if (DEBUG_VARS::DEBUG_TRACING) {
            debug_msg("Reducible Test");
        }

        if (f(g)) return true;
    }
    return false;
}

bool batch_colorable_test(const ListGraph& g) {
    vector<std::function<bool(const ListGraph&)>> tests = {alon_tarsi_test};

    for (auto f : tests) {
        if (DEBUG_VARS::DEBUG_TRACING) {
            debug_msg("Colorable Test");
        }

        if (f(g)) return true;
    }
    return false;
}

vector<int> minimal_irreducible_deletedvertices(const ListGraph& g) {
    for (int v = 0; v < g.n; ++v) {
        ListGraph gp = g.remove_vertex(v);
        if (DEBUG_VARS::DEBUG_TRACING) {
                debug_var(gp.al);
                debug_var(gp.list_sizes);
            }
        if(!batch_colorable_test(gp)) {
            if (DEBUG_VARS::DEBUG_TRACING) {
                debug_msg("Still does not pass tests");
            }

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

    

    if (DEBUG_VARS::DEBUG_TRACING) {

        debug_msg("recursive_redu_b_t");
        debug_var(g.n);
        debug_var(g.m);
        debug_var(g.al);
        debug_var(g.list_sizes);
        debug_msg(g.compute_list_code().to_string());
    }



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

        if (DEBUG_VARS::DEBUG_TRACING) {
            debug_msg("Already in memory");
        }

        return mem[code];
    }

    if (g.nocolors()) return mem[code] = false;

    for (int u = 0; u < g.n; ++u) {
        if (g.list_sizes[u] <= 0) {
            return mem[code] = recursive_reducibility_batch_test(g.precolor_vertex(u));
        }
    }

    if (batch_reducible_test(g)) {
        return mem[code] = true;
    }
    vector<int> deleted_vertices = minimal_irreducible_deletedvertices(g);

    vector<int> deleted(g.n);
    for (int v : deleted_vertices) deleted[v] = 1;
    int undel_vertex = 0;
    while (deleted[undel_vertex]) undel_vertex++;

    if (DEBUG_VARS::DEBUG_TRACING) {
        debug_var(undel_vertex);
    }

    ListGraph ng = g.precolor_vertex(undel_vertex);
    return mem[code] = recursive_reducibility_batch_test(ng);
}