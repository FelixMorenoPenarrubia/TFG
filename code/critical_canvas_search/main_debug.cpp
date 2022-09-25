#include<iostream>
#include<vector>
#include "search.cpp"

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using pg = PlaneGraph;


void generate_code() {
	cout << "Introduce n, m, l of the graph" << endl;
	int n, m, l;
	cin >> n >> m >> l;
	cout << "Introduce adjacency list (with edges duplicated)" << endl;
	vector<vector<int> > al(n);
	for (int i=0; i < 2*m; ++i) {
		int u, v;
		cin >> u >> v;
		al[u].push_back(v);
	}
	pg g = pg(l, al);
	cout << g.compute_code().to_string() << endl;
}

int main() {
	cout << "Select program: " << endl;
	cout << "1. Search" << endl;
	cout << "2. Generate code for graph" << endl;
	cout << "3. Add tripod to graph" << endl;
	cout << "4. Perform degree test" << endl;
	cout << "5. Perform deficiency test" << endl;
	cout << "6. Perform gadget 4 test" << endl;
	cout << "7. Perform biconnected components of degree 5 test" << endl;
	cout << "8. Perform gadget 5 test" << endl;
	
	int c;
	cin >> c;
	if(c == 1) {
		search();
	}
	if(c == 2) {
		generate_code();
	}
}
