#include<iostream>
#include<vector>
#include<string>
#include "PrecoloredPathGraph.hh"
#include "PrecoloredPathGraphSearch.hh"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::string;
using std::set;

int main() {
    PrecoloredPathGraphSearch search;
	set<PrecoloredPathGraphCode> s;

    for (int l=3; l <= 6; ++l) {
        auto ns = search.get_all_codes(l);
        for (auto x : ns) s.insert(x);
    }

	int T;
    cin >> T;
    for (int t=0; t < T; ++t) {
        PrecoloredPathGraph g = PrecoloredPathGraph::read(cin);
        if (s.find(g.compute_code()) == s.end()) {
            cout << t << endl;
        }
    }
}