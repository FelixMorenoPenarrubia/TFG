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

int main() {
    TwoPrecoloredPathsGraphSearch s;
	int d;
	cin >> d;
	vector<TwoPrecoloredPathsGraph> v = s.get(d);

	cout << v.size() << endl;
	for (TwoPrecoloredPathsGraph g : v) {
		g.write(cout);
		//cout << g.compute_code().to_string() << endl;
	}
}