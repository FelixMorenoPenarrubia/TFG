#include<iostream>
#include<vector>
#include<string>
#include "TwoPrecoloredPathsGraph.hh"
#include "TwoPrecoloredPathsGraphSearch.hh"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::string;
using std::set;

int main() {
    TwoPrecoloredPathsGraphSearch search;
    set<PrecoloredPathAndVertexGraphCode> G;
    const int D = 4;
	for (int d=1; d <= D; ++d) {
        vector<TwoPrecoloredPathsGraph> vtpp = search.get(d);
        if (d > 1) {
            vector<TwoPrecoloredPathsGraph> vtppdm1 = search.get(d-1);
            for (auto g : vtppdm1) vtpp.push_back(g);
        }
        set<PrecoloredPathAndVertexGraphCode> st;

        for (auto g1 : vtpp) {
            for (auto g2 : g1.unprecolor_vertices()) {
                if (g2.test_no_l3_adjacent() && g2.distance_between_paths() == d && g2.test_criticality()) {
                    G.insert(g2.compute_code());
                }
            }
        }
    }

    for (auto c1 : G) {
        PrecoloredPathAndVertexGraph g1 = PrecoloredPathAndVertexGraph(c1);
        for (auto c2 : G) {
            PrecoloredPathAndVertexGraph g2 = PrecoloredPathAndVertexGraph(c2);
            vector<PrecoloredPathAndVertexGraph> vg = PrecoloredPathAndVertexGraph::fuse_paths(g1, g2);
            for (auto g : vg) {
                if (g.distance_between_paths() > D && g.test_no_l3_adjacent()) {
                    if (g.test_criticality()) {
                        cout << "BAD GRAPH" << endl;
                        g1.write(cout);
                        cout << "---" << endl;
                        g2.write(cout);
                        cout << "---" << endl;
                        g.write(cout);
                        return 0;
                    }
                }
            }
        }
    }


	
}