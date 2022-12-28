#include<iostream>
#include<vector>
#include<string>
#include<set>
#include "PrecoloredPathGraph.hh"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

int main() {
    
	std::set<PrecoloredPathGraphCode> v;

    int T;
    cin >> T;
    //int s = 0;
    while (T--) {
        v.insert(PrecoloredPathGraph::read(cin).compute_code());
     //   cout << PrecoloredPathGraph::read(cin).compute_code().to_string() << endl;
       // s += PrecoloredPathGraph::read(cin).test_criticality();
    }
    //cout << s << endl;

	cout << v.size() << endl;
	for (auto c : v) {
		cout << c.to_string() << endl;
	}
}