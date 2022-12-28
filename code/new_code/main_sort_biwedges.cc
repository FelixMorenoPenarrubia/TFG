#include<iostream>
#include<vector>
#include<string>
#include<set>
#include "TwoPrecoloredPathsGraph.hh"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

int main() {
    
	std::set<TwoPrecoloredPathsGraphCode> v;

    int T;
    cin >> T;
    //int s = 0;
    for (int t=0; t < T; ++t) {
        v.insert(TwoPrecoloredPathsGraph::read(cin).compute_code());
     //   cout << PrecoloredPathGraph::read(cin).compute_code().to_string() << endl;
       // s += PrecoloredPathGraph::read(cin).test_criticality();
    }
    //cout << s << endl;

	cout << v.size() << endl;
	for (auto c : v) {
		cout << c.to_string() << endl;
	}
}