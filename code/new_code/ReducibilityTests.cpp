#include "ReducibilityTests.hh"
#include "DFSGraph.hh"
#include "AlonTarsi.hh"
#include "debug_utility.hh"
#include "Parallelism.hh"
#include "GlobalSettings.hh"
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

bool reducible_gadgets_test(const ListGraph& g) {
    vector<ListGraph> gadgets = {
        ListGraph( //Gadget4
            {
                {1, 2, 3},
                {2, 0},
                {3, 0, 1},
                {0, 2}
            },
            {3, 2, 2, 2}
        ), 
        ListGraph(  //Gadget5
            {
                {4, 3, 2, 1},
                {2, 0},
                {3, 0, 1},
                {4, 0, 2},
                {0, 3}
            },
            {4, 2, 2, 2, 2}
        ),
        ListGraph(
            {
                {1, 2, 3, 4, 5},
                {2, 0, 5},
                {3, 0, 1},
                {4, 0, 2},
                {5, 0, 3},
                {1, 0, 4}
            },
            {5, 3, 3, 2, 2, 2}
        )
    };

    for (auto& h : gadgets) {
        if (g.subgraph_test(h)) return true;
    }
    return false;
}

bool alon_tarsi_test(const ListGraph& g) {
    /*static map<ListGraphCode, bool> mem;
    
    ListGraphCode code = g.compute_list_code();
    #ifdef PARALLEL
    Parallelism::alon_tarsi_mutex.lock();
    #endif
    bool in_memory = mem.find(code) != mem.end();
    #ifdef PARALLEL
    Parallelism::alon_tarsi_mutex.unlock();
    #endif
    if (in_memory) {
        #ifdef PARALLEL
        Parallelism::alon_tarsi_mutex.lock();
        #endif
        bool ans = mem[code];
        #ifdef PARALLEL
        Parallelism::alon_tarsi_mutex.unlock();
        #endif
        return ans;
    } 
    bool ans = alon_tarsi(g);
    #ifdef PARALLEL
    Parallelism::alon_tarsi_mutex.lock();
    #endif
    mem[code] = ans;
    #ifdef PARALLEL
    Parallelism::alon_tarsi_mutex.unlock();
    #endif

    return ans;*/

    static map<ListGraphCode, bool> mem;

    const int ALON_TARSI_LIMIT_M = 33;
    if (g.m > ALON_TARSI_LIMIT_M) return false;

    if(GlobalSettings::ALON_TARSI_MEMOIZE) {
        ListGraphCode code = g.compute_list_code();
        if (mem.find(code) == mem.end()) {
            mem[code] = alon_tarsi(g);
        }
        return mem[code];
    }

    return alon_tarsi(g);
    
    /*
    ListGraphCode code = g.compute_list_code();
    bool ans = alon_tarsi(g); 
    if(mem.find(code) != mem.end()) {
        if (ans != mem[code]) {
            debug_msg("ERROR: answer differs for code " + code.to_string());
            g.write(std::cerr);
        }
    }
    else {
        mem[code] = ans;
    }
    return ans;
    */
}

bool color_and_collapse(const ListGraph& g) {
    if (g.n == 1) {
        return g.list_sizes[0] > 0;
    }
    for (int u=0; u < g.n; ++u) {
        if (g.list_sizes[u] <= 0) return false;
        if (g.list_sizes[u] == 1) return color_and_collapse(g.precolor_vertex(u));
        for (int v : g.al[u]) {
            if (g.list_sizes[v] < g.list_sizes[u]) return color_and_collapse(g.precolor_vertex_smart(u, v));
        }
    }
    return color_and_collapse(g.precolor_vertex(0));
}

bool color_and_collapse_test(const ListGraph& g) {
    if (color_and_collapse(g)) return true;
    const int N_REP = 10;
    std::mt19937 gen;
    for (int i=0; i < N_REP; ++i) {
        if (color_and_collapse(g.shuffle_order(gen))) return true;
    }
    return color_and_collapse(g.reverse_al_order_lg());
}

bool two_neighbors_heuristic(const ListGraph& g) {
    if (g.n == 1) {
        return g.list_sizes[0] > 0;
    }
    vector<ListGraph> vcc = g.connected_components();
    if (vcc.size() > 1) {
        for (ListGraph gi : vcc) {
            if (!two_neighbors_heuristic(gi)) return false;
        }
        return true;
    }
    const int ALON_TARSI_THRESHOLD = 20;
    if (g.m <= ALON_TARSI_THRESHOLD) {
        return alon_tarsi_test(g);
    }
    for (int u=0; u < g.n; ++u) {
        if (g.list_sizes[u] <= 0) return false;
    }
    for (int u=0; u < g.n; ++u) {
         if (g.list_sizes[u] == 1) return two_neighbors_heuristic(g.precolor_vertex(u));
    }

    for (int l=3; l <= 5; ++l) {
        for (int u=0; u < g.n; ++u) {
            if (g.list_sizes[u] <= l && g.list_sizes[u] >= 3) {
                for (int s=g.list_sizes[u]+1; s <= 2*g.list_sizes[u]-2; ++s) {
                    for (int v : g.al[u]) {
                        for (int w : g.al[u]) {
                            if (v == w) continue;
                            if (g.neighbors(v, w)) continue;
                            if (g.list_sizes[v] + g.list_sizes[w] > g.list_sizes[u] && g.list_sizes[v] + g.list_sizes[w] <= s && g.list_sizes[v] < g.list_sizes[u] && g.list_sizes[w] < g.list_sizes[u]) {
                                return two_neighbors_heuristic(g.precolor_vertices_smart_twoneighbors(u, v, w));
                            }
                        }
                    }
                }
            }
        }
    }
    for (int u=0; u < g.n; ++u) {
        for (int v : g.al[u]) {
            if (g.list_sizes[v] < g.list_sizes[u]) return two_neighbors_heuristic(g.precolor_vertex_smart(u, v));
        }
    }
    return two_neighbors_heuristic(g.precolor_vertex(0));


}

bool two_neighbors_heuristic_test(const ListGraph& g) {
    if (two_neighbors_heuristic(g)) return true;
    const int N_REP = 10;
    std::mt19937 gen;
    for (int i=0; i < N_REP; ++i) {
        if (two_neighbors_heuristic(g.shuffle_order(gen))) return true;
    }
    return two_neighbors_heuristic(g.reverse_al_order_lg());
}

bool hill_climbing_two_neighbors_test(const ListGraph& g) {
    if (g.n == 1) {
        return g.list_sizes[0] > 0;
    }
    for (int u=0; u < g.n; ++u) {
        if (g.list_sizes[u] <= 0) return false;
    }
    vector<ListGraph> vcc = g.connected_components();
    if (vcc.size() > 1) {
        for (ListGraph gi : vcc) {
            if (!two_neighbors_heuristic(gi)) return false;
        }
        return true;
    }
    const int ALON_TARSI_THRESHOLD = 20;
    if (g.m <= ALON_TARSI_THRESHOLD) {
        return alon_tarsi_test(g);
    }

    std::function<int(const ListGraph&)> score_fun = [ALON_TARSI_THRESHOLD, &score_fun] (const ListGraph& g) -> int {
        if (g.n == 1) {
            return g.list_sizes[0] > 0 ? 1e9 : -1e9;
        }
        vector<ListGraph> vcc = g.connected_components();
        if (vcc.size() > 1) {
            int cs = 1e9;
            for (ListGraph gi : vcc) {
                cs = std::min(cs, score_fun(gi));
            }
            return cs;
        }

        if(g.m <= ALON_TARSI_THRESHOLD) {
            return alon_tarsi_test(g) ? 1e9 : -1e9;
        }
        for (int u=0; u < g.n; ++u) {
           if (g.list_sizes[u] <= 0) return -1e9;
        }
        int cs = -g.n*g.n-g.m;

        for (int u = 0; u < g.n; ++u) {
            if (g.list_sizes[u] == 1) cs -= 50;
            for (int v : g.al[u]) {
                cs -= (6-g.list_sizes[u])*(6-g.list_sizes[v]);
            }
        }

        return cs;
    };

    int best_score = -2e9;
    ListGraph best_graph = g.precolor_vertex(0);

    auto add_graph = [&score_fun, &best_score, &best_graph] (const ListGraph& g) -> void {
        int g_score = score_fun(g);
        if (g_score > best_score) {
            best_score = g_score;
            best_graph = g;
        }
    };

    for (int u=0; u < g.n; ++u) {
        add_graph(g.precolor_vertex(u));
        if ((int)g.al[u].size() < g.list_sizes[u]) {
            add_graph(g.remove_vertex(u));
        }
    }

    for (int u=0; u < g.n; ++u) {    
        for (int v : g.al[u]) {
            for (int w : g.al[u]) {
                if (v == w) continue;
                if (g.neighbors(v, w)) continue;
                if (g.list_sizes[v] + g.list_sizes[w] > g.list_sizes[u] && g.list_sizes[v] <= g.list_sizes[u] && g.list_sizes[w] <= g.list_sizes[u]) {
                    add_graph(g.precolor_vertices_smart_twoneighbors(u, v, w));
                }
            }
        }     
    }
    
    for (int u=0; u < g.n; ++u) {
        for (int v : g.al[u]) {
            if (g.list_sizes[v] < g.list_sizes[u]) add_graph(g.precolor_vertex_smart(u, v));
        }
    }

    return hill_climbing_two_neighbors_test(best_graph);
}

bool batch_reducible_test(const ListGraph& g) {
    vector<std::function<bool(const ListGraph&)>> tests = {degree_test, biconnected_components_degreeassignment_test, color_and_collapse_test, reducible_gadgets_test, two_neighbors_heuristic_test, hill_climbing_two_neighbors_test, alon_tarsi_test};

    for (auto f : tests) {
        if (DEBUG_VARS::DEBUG_TRACING) {
            debug_msg("Reducible Test");
        }

        if (f(g)) return true;
    }
    return false;
}

bool batch_colorable_test(const ListGraph& g) {
    vector<std::function<bool(const ListGraph&)>> tests = {color_and_collapse_test, two_neighbors_heuristic_test, hill_climbing_two_neighbors_test, alon_tarsi_test};

    vector<ListGraph> vcc = g.connected_components();

    if (vcc.size() > 1) {
        for (ListGraph gi : vcc) {
            if (!batch_colorable_test(gi)) return false;
        }
        return true;
    }

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

bool recursive_colorability_test(const ListGraph& g) {
    if (g.empty()) return false;
    if (g.nocolors()) {
        return false;
    }

    vector<ListGraph> vcc = g.connected_components();

    if (vcc.size() > 1) {
        for (ListGraph gi : vcc) {
            if (recursive_colorability_test(gi)) return true;
        }
        return false;
    }

    for (int u = 0; u < g.n; ++u) {
        if (g.list_sizes[u] <= 0) {
            return recursive_colorability_test(g.precolor_vertex(u));
        }
    }
    
     for (int u=0; u < g.n; ++u) {
        if (g.list_sizes[u] == 1) {
            for (int v : g.al[u]) {
                if (g.list_sizes[v] == 1) {
                    return recursive_colorability_test(g.precolor_vertex(u));
                }
            }
        }
    }
    

    if (batch_colorable_test(g)) {
        return true;
    }

   

    vector<int> deleted_vertices = minimal_irreducible_deletedvertices(g);

    vector<int> deleted(g.n);
    for (int v : deleted_vertices) deleted[v] = 1;
    int undel_vertex = 0;
    while (deleted[undel_vertex]) undel_vertex++;

    return recursive_colorability_test(g.precolor_vertex(undel_vertex)); 
}

bool batch_test(const ListGraph& g) {
    
    if (g.empty()) return false;

    if (g.nocolors()) return false; //TODO: necessary because Alon-Tarsi does not work correctly otherwise, fix?
    

    vector<ListGraph> vcc = g.connected_components();

    if (vcc.size() > 1) {
        for (ListGraph gi : vcc) {
            if (batch_test(gi)) return true;
        }
        return false;
    }

    if (batch_reducible_test(g)) {
        return true;
    }
    if (recursive_colorability_test(g)) {
        return true;
    }
    return false;
}