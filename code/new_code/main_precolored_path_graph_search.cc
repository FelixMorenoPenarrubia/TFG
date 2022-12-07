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

int main() {
    PrecoloredPathGraphSearch s;
	int l;
	cin >> l;
	vector<PrecoloredPathGraph> v = s.get_all(l);

	cout << v.size() << endl;
	for (PrecoloredPathGraph g : v) {
		g.write(cout);
	}
}