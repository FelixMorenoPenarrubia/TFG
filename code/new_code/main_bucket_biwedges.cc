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
    
	std::map<TwoPrecoloredPathsGraphCode, vector<int>> v;

    int T;
    cin >> T;
    //int s = 0;
    for (int t=0; t < T; ++t) {
        v[TwoPrecoloredPathsGraph::read(cin).compute_code()].push_back(t);
    
    }
    //cout << s << endl;

	cout << v.size() << endl;
	for (auto c : v) {
		cout << c.first.to_string() << endl;
        for (int x : c.second) {
            cout << x << " ";
        }
        cout << endl;
	}
}