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

void print_all_orientations() {
	//n, m
	int n, m;
	cin >> n >> m;
	vector<pair<int, int> > el;
	//edge list - not duplicated
	for (int i=0; i < m; ++i) {
		int u, v;
		cin >> u >> v;
		el.emplace_back(u, v);
	}
	//n ints - bounds
	vector<int> bounds(n);
	for(int i=0; i < n; ++i) {
		cin >> bounds[i];
	}
	//one int (0 or 1) - equality
	bool equality;
	cin >> equality;

	OrientationGraph g = OrientationGraph(n, el);
	vector<vector<int> > ans = g.find_orientations(bounds, equality, false);
	for(vector<int> v : ans) {
		for(int i=0; i < (int)v.size(); ++i) {
			if (i > 0) cout << " ";
			cout << v[i];
		}
		cout << endl;
	}
}

void weak_alon_tarsi_test() {
	pg g = read_planegraph();
	cout << g.weak_alon_tarsi_test() << endl;
}

void strong_alon_tarsi_test() {
	pg g = read_planegraph();
	cout << g.strong_alon_tarsi_test() << endl;
}

void bounds_generation() {
	int n;
	int sum;
	cin >> n >> sum;
	vector<int> vec(n);
	for (int i=0; i < n; ++i) {
		cin >> vec[i];
	}
	vector<vector<int> > ans = PlaneGraph::bounds_generation(vec, sum);
	for(vector<int> bnd : ans) {
		for (int i=0; i < (int)bnd.size(); ++i) {
			if (i > 0) cout << " ";
			cout << bnd[i];
		}
		cout << endl;
	}
}

int main() {
	cerr << "Select program: " << endl;
	cerr << "1. Search" << endl;
	cerr << "2. Generate code for graph" << endl;
	cerr << "3. Isomorphism test for two canvases (adjacency list)" << endl;
	cerr << "4. Isomorphism test for two canvases (list of operations)" << endl;
	cerr << "5. Number of biconnected components of graph" << endl;
	cerr << "6. Perform biconnected components of degree 5 test" << endl;
	cerr << "7. Print all orientations with given bounds of graph" << endl;
	cerr << "8. Perform (weak) Alon-Tarsi test" << endl;
	cerr << "9. Perform (strong) Alon-Tarsi test" << endl;
	cerr << "10. Bounds generation" << endl;
	
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
	if(c == 7) {
		print_all_orientations();
	}
	if(c == 8) {
		weak_alon_tarsi_test();
	}
	if(c == 9) {
		strong_alon_tarsi_test();
	}
	if(c == 10) {
		bounds_generation();
	}
}
