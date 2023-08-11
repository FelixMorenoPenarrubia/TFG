#include<iostream>
#include<vector>
#include<string>
#include "PrecoloredPathGraph.hh"
#include "RestrictedPrecoloredPathGraphSearch.hh"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

int main() {
    RestrictedPrecoloredPathGraphSearch s;
	int l;
	cin >> l;
	vector<PrecoloredPathGraph> v = s.get_all(l);

	
	cout << v.size() << endl;
	for (PrecoloredPathGraph g : v) {
		g.write(cout);
	}
	
	
	/*
	int maxn = 0;
	PrecoloredGraph mg;
	for (PrecoloredPathGraph g : v) {
		if (g.n+g.m > maxn) {
			maxn = g.n+g.m;
			mg = g;
		}
	}
	mg.write(cout);
	*/
	

}