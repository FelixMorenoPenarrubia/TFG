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

    for (auto c1 : G) {
        TwoPrecoloredPathsGraph g1 = TwoPrecoloredPathsGraph(c1);
        for (auto c2 : G) {
            TwoPrecoloredPathsGraph g2 = TwoPrecoloredPathsGraph(c2);
            vector<TwoPrecoloredPathsGraph> vg = TwoPrecoloredPathsGraph::fuse_paths(g1, g2);
            for (auto g : vg) {
                if (G.find(g.compute_code()) == G.end()) {
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