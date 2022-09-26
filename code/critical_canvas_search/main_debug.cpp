#include<iostream>
#include<vector>
#include "search.cpp"

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using pg = PlaneGraph;


pg read_planegraph() {
	//cout << "Introduce n, m, l of the graph" << endl;
	int n, m, l;
	cin >> n >> m >> l;
	//cout << "Introduce adjacency list (with edges duplicated)" << endl;
	vector<vector<int> > al(n);
	for (int i=0; i < 2*m; ++i) {
		int u, v;
		cin >> u >> v;
		al[u].push_back(v);
	}
	return pg(l, al);
}

void generate_code() {
	pg g = read_planegraph();
	cout << g.compute_code().to_string() << endl;
}

void num_biconnected_components() {
	//n, m
	int n, m;
	cin >> n >> m;
	//edges not duplicated
	vector<vector<int> > al(n);
	for (int i=0; i < m; ++i) {
		int u, v;
		cin >> u >> v;
		al[u].push_back(v);
		al[v].push_back(u);
	}
	DFSGraph g;
	g.n = n;
	g.al = al;
	vector<DFSGraph> comp = g.partition_biconnected();
	cout << comp.size() << endl;


}

void biconnected_deg5_components_test() {
	pg g = read_planegraph();
	cout << g.biconnected_deg5_components_test() << endl;
}

int main() {
	cerr << "Select program: " << endl;
	cerr << "1. Search" << endl;
	cerr << "2. Generate code for graph" << endl;
	cerr << "3. Isomorphism test for two canvases (adjacency list)" << endl;
	cerr << "4. Isomorphism test for two canvases (list of operations)" << endl;
	cerr << "5. Number of biconnected components of graph" << endl;
	cerr << "6. Perform biconnected components of degree 5 test" << endl;
	
	int c;
	cin >> c;
	if(c == 1) {
		search();
	}
	if(c == 2) {
		generate_code();
	}
	if(c == 5) {
		num_biconnected_components();
	}
	if(c == 6) {
		biconnected_deg5_components_test();
	}
}
