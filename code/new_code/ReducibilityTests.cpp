#include "ReducibilityTests.hh"
#include "DFSGraph.hh"
#include "AlonTarsi.hh"
#include "debug_utility.hh"
#include "Parallelism.hh"
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
    static map<ListGraphCode, bool> mem;
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

    return ans;
}

bool batch_reducible_test(const ListGraph& g) {
    vector<std::function<bool(const ListGraph&)>> tests = {degree_test, biconnected_components_degreeassignment_test, reducible_gadgets_test, alon_tarsi_test};

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

    #ifdef PARALLEL
    Parallelism::recursive_reducibility_test_mutex.lock();
    #endif
    bool in_memory = mem.find(code) != mem.end();
    #ifdef PARALLEL
    Parallelism::recursive_reducibility_test_mutex.unlock();
    #endif

    if (in_memory) {

        if (DEBUG_VARS::DEBUG_TRACING) {
            debug_msg("Already in memory");
        }
        #ifdef PARALLEL
        Parallelism::recursive_reducibility_test_mutex.lock();
        #endif
        bool ans = mem[code];
        #ifdef PARALLEL
        Parallelism::recursive_reducibility_test_mutex.unlock();
        #endif

        return ans;
    }

    auto setmem = [&] (bool x) {
        #ifdef PARALLEL
        Parallelism::recursive_reducibility_test_mutex.lock();
        #endif
        mem[code] = x;
        #ifdef PARALLEL
        Parallelism::recursive_reducibility_test_mutex.unlock();
        #endif
    };

    if (g.nocolors()) {

        setmem(false);
        return false;
    }

    for (int u = 0; u < g.n; ++u) {
        if (g.list_sizes[u] <= 0) {

            bool ans = recursive_reducibility_batch_test(g.precolor_vertex(u));
            setmem(ans);
            return ans;
        }
    }

    if (batch_reducible_test(g)) {
        setmem(true);
        return true;
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
    bool ans = recursive_reducibility_batch_test(ng);
    setmem(ans);
    return ans;
}