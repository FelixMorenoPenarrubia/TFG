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
    
	vector<PrecoloredPathGraph> v;

    int T;
    cin >> T;
    for (int t=0; t < T; ++t) {
        PrecoloredPathGraph g = PrecoloredPathGraph::read(cin);
        g.list_sizes[0] = 2;
        g.list_sizes[4] = 2;

        vector<int> morph(g.n);
        for (int i=1; i < g.n; ++i) {
            morph[i] = i-1;
        }
        morph[0] = g.n-1;

        vector<vector<int> > nal(g.n); 
        vector<int> nls(g.n);

        for (int u=0; u < g.n; ++u) {
            nls[morph[u]] = g.list_sizes[u];
            for (int v : g.al[u]) {
                nal[morph[u]].push_back(morph[v]);
            }
        }

        PrecoloredPathGraph pg = PrecoloredPathGraph(nal, 3, nls);
        if (pg.test_criticality()) {
            v.push_back(pg);
        }
        
     
    }
   

	cout << v.size() << endl;
	for (auto g : v) {
		g.write(cout);
	}
}