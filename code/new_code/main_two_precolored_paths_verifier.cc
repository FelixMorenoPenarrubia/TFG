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
    set<TwoPrecoloredPathsGraphCode> G;
    const int D = 4;
	for (int d=1; d <= D; ++d) {
        auto s = search.get_codes(d);
        for (auto c : s) G.insert(c);
    }

    vector<set<TwoPrecoloredPathsGraphCode>> by_dist(5);

    for (auto c1 : G) {
        TwoPrecoloredPathsGraph g1 = TwoPrecoloredPathsGraph(c1);
        for (auto c2 : G) {
            TwoPrecoloredPathsGraph g2 = TwoPrecoloredPathsGraph(c2);
            vector<TwoPrecoloredPathsGraph> vg = TwoPrecoloredPathsGraph::fuse_paths(g1, g2);
            for (auto g : vg) {
                if (g.count_l3_adjacent_pairs() == 1) {
                    /*
                    if (g.test_criticality()) {
                        cout << "GRAPH" << endl;
                        g1.write(cout);
                        cout << "---" << endl;
                        g2.write(cout);
                        cout << "---" << endl;
                        g.write(cout);
                        cout << "---" << endl;
                        //return 0;
                    }
                    */
                   by_dist[g.distance_between_paths()].insert(g.compute_code());

                }
            }
        }
    }

    for (int i=2; i <= 4; ++i) {
        cout << "DISTANCE " << i << ": " << by_dist[i].size() << endl;
        for (auto c : by_dist[i]) {
            TwoPrecoloredPathsGraph(c).write(cout);
        }
    }   


	
}