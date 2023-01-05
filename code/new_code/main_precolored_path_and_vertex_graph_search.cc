#include<iostream>
#include<vector>
#include<set>
#include<string>
#include "TwoPrecoloredPathsGraph.hh"
#include "TwoPrecoloredPathsGraphSearch.hh"
#include "PrecoloredPathAndVertexGraph.hh"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::set;
using std::string;

int main() {
    TwoPrecoloredPathsGraphSearch search;
	int d;
	cin >> d;
	vector<TwoPrecoloredPathsGraph> vtpp = search.get(d);
    if (d > 1) {
        vector<TwoPrecoloredPathsGraph> vtppdm1 = search.get(d-1);
        for (auto g : vtppdm1) vtpp.push_back(g);
    }
    set<PrecoloredPathAndVertexGraphCode> st;

    for (auto g1 : vtpp) {
        for (auto g2 : g1.unprecolor_vertices()) {
            if (g2.test_no_l3_adjacent() && g2.distance_between_paths() == d && g2.test_criticality()) {
                st.insert(g2.compute_code());
            }
        }
    }

	cout << st.size() << endl;
	for (PrecoloredPathAndVertexGraphCode c : st) {
		PrecoloredPathAndVertexGraph(c).write(cout);
	}
}