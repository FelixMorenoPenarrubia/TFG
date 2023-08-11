#include<iostream>
#include<vector>
#include<string>
#include "Canvas.hh"
#include "CanvasSearch.hh"
#include "TwoTriangleGraph.hh"
#include "ReducibilityTests.hh"
#include "PrecoloredPathGraph.hh"
#include "PrecoloredPathGraphSearch.hh"
#include "TwoPrecoloredPathsGraph.hh"
#include "TwoPrecoloredPathsGraphSearch.hh"
#include "PrecoloredPathAndTriangleGraph.hh"
#include "RestrictedPrecoloredPathGraphSearch.hh"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::string;
using pg = Canvas;

//TODO: refactor this call
pg read_planegraph() {
	/*
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
	return pg(al, l);
	*/
	return Canvas::read(cin);
}

void canvas_search() {
	CanvasSearch s;
	int l;
	cin >> l;
	std::set<CanvasCode>& st = s.get_chordless_code(l);

	cout << st.size() << endl;
	for (auto c : st) {
		cout << c.to_string() << endl;
	}
}

void pp_search() {
	PrecoloredPathGraphSearch s;
	int l;
	cin >> l;
	vector<PrecoloredPathGraph> v = s.get_all(l);

	cout << v.size() << endl;
	for (PrecoloredPathGraph g : v) {
		g.write(cout);
	}
}

void tpp_search() {
	TwoPrecoloredPathsGraphSearch s;
	int d;
	cin >> d;
	vector<TwoPrecoloredPathsGraph> v = s.get(d);

	cout << v.size() << endl;
	for (TwoPrecoloredPathsGraph g : v) {
		g.write(cout);
	}
}

void generate_code() {
	pg g = read_planegraph();
	cout << g.compute_code().to_string() << endl;
}

void print_graph_from_code() {
	string s;
	cin >> s;
	pg g = Canvas(CanvasCode(s));
	g.write(cout);
}

void isomorphism_test_adjacency_list() {
	pg g1 = read_planegraph();
	pg g2 = read_planegraph();
	//cerr << g1.compute_code().to_string() << endl;
	//cerr << g2.compute_code().to_string() << endl;
	cout << (g1.compute_code().to_string() == g2.compute_code().to_string()) << endl;
}

void isomorphism_test_operation_list() {
	vector<pg> vpg;
	vpg.push_back(read_planegraph());
	vpg.push_back(read_planegraph());
	//Number of operations
	int q;
	cin >> q;
	for(int i=0; i < q; ++i) {

		cerr << "Operation " << i << endl;

		string s;
		cin >> s;
		if (s == "add_tripod") {
			//add_tripod idx s j bm
			int idx, s, j;
			long long bm;
			cin >> idx >> s >> j >> bm;
			vpg.push_back(vpg[idx].add_tripod(s, j, bm));
		}
		else if (s == "fuse_chord") {
			//fuse_chord idx1 idx2 j1 j2 same_orientation
			int idx1,idx2,j1,j2,same_orientation;
			cin >> idx1 >> idx2 >> j1 >> j2 >> same_orientation;
			vpg.push_back(Canvas::fuse_chord(vpg[idx1], vpg[idx2], j1, j2, same_orientation));
		}
		else if (s == "cycle_subgraph") {
			/*//cycle_subgraph idx cs v1 ... vcs
			int idx, cs;
			cin >> idx >> cs;
			vector<int> cyc(cs);
			for (int i=0; i < cs; ++i) {
				cin >> cyc[i];
			}
			vpg.push_back(vpg[idx].subgraph_contained_in_cycle(cyc));
            */
		}
		else if (s == "read_code") {
			//read_code code
			string code;
			cin >> code;
			vpg.push_back(Canvas(CanvasCode(code)));
		}
		else if (s == "print_isomorphism") {
			//print_isomorphism idx1 idx2
			int idx1, idx2;
			cin >> idx1 >> idx2;
			cout << (vpg[idx1].compute_code().to_string() == vpg[idx2].compute_code().to_string()) << endl;
		}
	}

	/*for (int i=0; i < (int)vpg.size(); ++i) { 
		pg g = vpg[i];
		cerr << i << ": " << endl;
		print_canvas(g);
	}*/
}

void isomorphism_test_operation_list_precoloredpathgraph() {
	vector<PrecoloredPathGraph> vppg;
	vector<TwoPrecoloredPathsGraph> vtppg;
	vppg.push_back(PrecoloredPathGraph::read(cin));
	vppg.push_back(PrecoloredPathGraph::read(cin));
	//Number of operations
	int q;
	cin >> q;
	for(int i=0; i < q; ++i) {

		string s;
		cin >> s;
		if (s == "add_tripod") {
			//add_tripod idx s j bm
			int idx, s, j;
			long long bm;
			cin >> idx >> s >> j >> bm;
			vppg.push_back(vppg[idx].add_tripod(s, j, bm));
		}
		else if (s == "fuse_chord") {
			//fuse_chord idx1 idx2 ls
			int idx1,idx2,ls;
			cin >> idx1 >> idx2 >> ls;
			vppg.push_back(PrecoloredPathGraph::fuse_chord(vppg[idx1], vppg[idx2], ls));
		}
		else if (s == "print_isomorphism") {
			//print_isomorphism idx1 idx2
			int idx1, idx2;
			cin >> idx1 >> idx2;
			cout << (vppg[idx1].compute_code().to_string() == vppg[idx2].compute_code().to_string()) << endl;
		}
		else if (s == "empty_cycle") {
			int n, l;
			cin >> n >> l;
			vector<int> ls(n);
			for (int& x : ls) cin >> x;
			vppg.push_back(PrecoloredPathGraph::empty_cycle(l, ls));
		}
		else if (s == "read_graph") {
			vppg.push_back(PrecoloredPathGraph::read(cin));
		}
		else if (s == "read_code") {
			vppg.push_back(PrecoloredPathGraph::read_code(cin));
		}
		else if (s == "read_tpp_graph") {
			vtppg.push_back(TwoPrecoloredPathsGraph::read(cin));
		}
		else if (s == "print_isomorphism_tpp") {
			int idx1, idx2;
			cin >> idx1 >> idx2;
			cout << (vtppg[idx1].compute_code().to_string() == vtppg[idx2].compute_code().to_string()) << endl;
		}
		else if (s == "fuse_pp_sameside") {
			//with lists of size 5
			int idx1, idx2;
			cin >> idx1 >> idx2;
			vtppg.push_back(TwoPrecoloredPathsGraph::fuse_precoloredpaths_sameside(vppg[idx1], vppg[idx2]).back());
		}
		else if (s == "fuse_pp_oppositeside") {
			//with lists of size 5
			int idx1, idx2;
			cin >> idx1 >> idx2;
			vtppg.push_back(TwoPrecoloredPathsGraph::fuse_precoloredpaths_oppositeside(vppg[idx1], vppg[idx2]).back());
		}
	}

	for (int i=0; i < (int)vppg.size(); ++i) { 
		cerr << i << ": " << vppg[i].compute_code().to_string() << endl;
	}

	for (int i=0; i < (int)vtppg.size(); ++i) { 
		cerr << i << ": " << vtppg[i].compute_code().to_string() << endl;
	}
}

void num_biconnected_components() {
    /*
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
    */

}

void biconnected_deg5_components_test() {
	pg g = read_planegraph();
	//cout << g.biconnected_deg5_components_test() << endl;
}

void print_all_orientations() {
    /*
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
    */
}

void weak_alon_tarsi_test() {
	pg g = read_planegraph();
	//cout << g.weak_alon_tarsi_test() << endl;
}

void strong_alon_tarsi_test() {
	pg g = read_planegraph();
	//cout << g.strong_alon_tarsi_test() << endl;
}

void bounds_generation() {
    /*
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
    */
}

void canvas_test() {
	pg g = read_planegraph();
	cout << g.test_criticality() << endl;
}

void recursive_alon_tarsi_test() {
	pg g = read_planegraph();
	//cout << g.recursive_reducibility_alon_tarsi_test() << endl;
}

void generate_two_triangle_graphs() {
	pg g = read_planegraph();
	vector<TwoTriangleGraph> v = TwoTriangleGraph::generate_from_canvas(g);
	for (auto t : v) {
		t.write(cout);
		cout << t.compute_code().to_string() << endl;
	}
}

void generate_critical_two_triangle_graphs() {
	 map<TwoTriangleGraphCode, TwoTriangleGraph> m;
	Canvas c = Canvas::read_code(cin);
       
        
        vector<TwoTriangleGraph> vec = TwoTriangleGraph::generate_from_canvas(c);

        for (TwoTriangleGraph g : vec) {

            g.compute_code();

            if (m.find(g.compute_code()) == m.end()) {
                if (g.test_criticality()) {
                    m[g.compute_code()] = g;
                }
            }
		}
	cout << m.size() << endl;
    for (auto p : m) {
        p.second.write(cout);
    }
}

void print_interior_list_graph_canvas() {
	Canvas::read(cin).compute_list_graph().write(cout);
}

void print_interior_list_graph_ttg() {
	TwoTriangleGraph::read(cin).compute_list_graph().write(cout);
}

void recursive_reducibility_test_list_graph() {
	cout << batch_test(ListGraph::read(cin)) << endl;
}

void colorability_test_list_graph() {
	cout << batch_colorable_test(ListGraph::read(cin)) << endl;
}

void two_triangle_graph_test() {
	cout << TwoTriangleGraph::read(cin).test_criticality() << endl;
}

void print_twotrianglegraph_from_code() {
	string s;
	cin >> s;
	TwoTriangleGraph(TwoTriangleGraphCode(s)).write(cout);
}

void generate_code_twotrianglegraph() {
	cout << TwoTriangleGraph::read(cin).compute_code().to_string() << endl;
}

void pp_test() {
	cout << PrecoloredPathGraph::read(cin).test_criticality() << endl;
}

void print_precoloredpathgraph_from_code() {
	string s;
	cin >> s;
	PrecoloredPathGraph(PrecoloredPathGraphCode(s)).write(cout);
}

void generate_code_twoprecoloredpathgraph() {
	cout << TwoPrecoloredPathsGraph::read(cin).compute_code().to_string() << endl;
}

void print_twoprecoloredpathgraph_from_code() {
	TwoPrecoloredPathsGraph::read_code(cin).write(cout);
}

void twoprecoloredpathgraph_test() {
	cout << TwoPrecoloredPathsGraph::read(cin).test_criticality() << endl;
}

void generate_code_precoloredpathgraph() {
	cout << PrecoloredPathGraph::read(cin).compute_code().to_string() << endl;
}

void fuse_precoloredpaths_sameside() {
	PrecoloredPathGraph g1 = PrecoloredPathGraph::read(cin);
	PrecoloredPathGraph g2 = PrecoloredPathGraph::read(cin);
	vector<TwoPrecoloredPathsGraph> vtppg = TwoPrecoloredPathsGraph::fuse_precoloredpaths_sameside(g1, g2);
	for (auto g : vtppg) {
		g.write(cout);
	}
}

void fuse_twotrianglegraphs() {
	TwoTriangleGraph g1 = TwoTriangleGraph::read(cin);
	TwoTriangleGraph g2 = TwoTriangleGraph::read(cin);
	vector<TwoTriangleGraph> ans = TwoTriangleGraph::fuse_triangles(g1, g2);
	for (TwoTriangleGraph g : ans) {
		g.write(cout);
	}
}

void fuse_canvas_and_precoloredpathgraph_triangle_path() {
	Canvas c = Canvas::read(cin);
	PrecoloredPathGraph p = PrecoloredPathGraph::read(cin);
	vector<PrecoloredPathAndTriangleGraph> v = PrecoloredPathAndTriangleGraph::fuse_canvas_and_path_triangle_in_path(c, p);
	for (auto g : v) {
	//	g.write(cout);
	}
	v[0].write(cout);
}

void fuse_canvas_and_precoloredpathgraph_triangle_canvas() {
	Canvas c = Canvas::read(cin);
	PrecoloredPathGraph p = PrecoloredPathGraph::read(cin);
	vector<PrecoloredPathAndTriangleGraph> v = PrecoloredPathAndTriangleGraph::fuse_canvas_and_path_triangle_in_canvas(c, p);
	for (auto g : v) {
	//	g.write(cout);
	}
	v[0].write(cout);
}

void fan_length_in_endpoints() {
	cout << PrecoloredPathGraph::read(cin).fan_length_in_endpoints() << endl;
}

void restricted_precolored_path_graph_search() {
	RestrictedPrecoloredPathGraphSearch s;
	int l;
	cin >> l;
	vector<PrecoloredPathGraph> v = s.get_all(l);

	cout << v.size() << endl;
	for (PrecoloredPathGraph g : v) {
		g.write(cout);
	}
}

void has_large_fans_or_bellows() {
	cout << PrecoloredPathGraph::read(cin).has_large_fans_or_bellows() << endl;
}

void largest_left_cut() {
	PrecoloredPathGraph::read(cin).largest_left_endpoint_cut().write(cout);
}

void fuse_pp_and_canvas_plan2() {
	PrecoloredPathGraph pp = PrecoloredPathGraph::read(cin);
	Canvas c = Canvas::read(cin);
	auto v = pp.fuse_canvas(c);
	for (auto g : v) {
		g.write(cout);
	}
}

void two_triangle_identification() {
	TwoTriangleGraph g = TwoTriangleGraph::read(cin);
	for (auto gt : g.identify_triangles()) {
		gt.write(cout);
	}
}

void set_outer_face() {
	auto g = TwoTriangleGraph::read(cin);
	g.set_first_triangle_as_outer_face();
	g.write(cout);
}


int main() {
	cerr << "Select program: " << endl;
	cerr << "1. Canvas Search" << endl;
	cerr << "2. Generate code for canvas" << endl;
	cerr << "3. Isomorphism test for two canvases (adjacency list)" << endl;
	cerr << "4. Isomorphism test for two canvases (list of operations)" << endl;
	//cerr << "5. Number of biconnected components of graph" << endl;
	//cerr << "6. Perform biconnected components of degree 5 test" << endl;
	//cerr << "7. Print all orientations with given bounds of graph" << endl;
	//cerr << "8. Perform (weak) Alon-Tarsi test" << endl;
	//cerr << "9. Perform (strong) Alon-Tarsi test" << endl;
	//cerr << "10. Bounds generation" << endl;
	cerr << "11. Canvas criticality test" << endl;
	//cerr << "12. Perform recursive Alon-Tarsi test" << endl;
	cerr << "13. Print Canvas from code" << endl;
	cerr << "14. Generate two-triangle-graphs from canvas" << endl;
	cerr << "15. Generate critical two-triangle-graphs from canvas CODE" << endl;
	cerr << "16. Print interior ListGraph from Canvas" << endl;
	cerr << "17. Recursive reducibility on ListGraph" << endl;
	cerr << "18. Colorability batch test on ListGraph" << endl;
	cerr << "19. Criticality test on TwoTriangleGraph" << endl;
	cerr << "20. Print interior ListGraph from TwoTriangleGraph" << endl;
	cerr << "21. Print TwoTriangleGraph from code" << endl;
	cerr << "22. Generate code for TwoTriangleGraph" << endl;
	cerr << "23. Isomorphism test for PrecoloredPathGraphs and TwoPrecoloredPathsGraphs (list of operations)" << endl;
	cerr << "24. Precolored Path Graph search" << endl;
	cerr << "25. Precolored Path Graph criticality test" << endl;
	cerr << "26. Two Precolored Path Graph search" << endl;
	cerr << "27. Print PrecoloredPathGraph from code" << endl;
	cerr << "28. Generate code for TwoPrecoloredPathGraph" << endl;
	cerr << "29. Print TwoPrecoloredPathGraph from code" << endl;
	cerr << "30. Critical test for TwoPrecoloredPathGraph" << endl;
	cerr << "31. Generate code for PrecoloredPathGraph" << endl;
	cerr << "32. Fuse two precolored paths on the same side to generate TwoPrecoloredPathGraph" << endl;
	cerr << "33. Fuse two TwoTriangleGraphs" << endl;
	cerr << "34. Fuse Canvas and PrecoloredPathGraph with triangle in path" << endl;
	cerr << "35. Fuse Canvas and PrecoloredPathGraph with triangle in canvas" << endl;
	cerr << "36. Fan length in endpoints of PrecoloredPathGraph" << endl;
	cerr << "37. Restricted PrecoloredPathGraph search" << endl;
	cerr << "38. PrecoloredPathGraph has large fans or bellows" << endl;
	cerr << "39. PrecoloredPathGraph largest left cut" << endl;
	cerr << "40. Fuse PrecoloredPathGraph and Canvas plan2" << endl;
	cerr << "41. Identify triangles from TwoTriangleGraph" << endl;
	cerr << "42. Set first triangle as outer face" << endl;
	
	int c;
	cin >> c;
	if(c == 1) {
		canvas_search();
	}
	if(c == 2) {
		generate_code();
	}
	if(c == 3) {
		isomorphism_test_adjacency_list();
	}
	if(c == 4) {
		isomorphism_test_operation_list();
	}
	/*if(c == 5) {
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
	}*/
	if(c == 11) {
		canvas_test();
	}
	/*if(c == 12) {
		recursive_alon_tarsi_test();
	}*/
	if(c == 13) {
		print_graph_from_code();
	}
	if(c == 14) {
		generate_two_triangle_graphs();
	}
	if(c == 15) {
		generate_critical_two_triangle_graphs();
	}
	if(c == 16) {
		print_interior_list_graph_canvas();
	}
	if(c == 17) {
		recursive_reducibility_test_list_graph();
	}
	if(c == 18) {
		colorability_test_list_graph();
	}
	if(c == 19) {
		two_triangle_graph_test();
	}
	if(c == 20) {
		print_interior_list_graph_ttg();
	}
	if(c == 21) {
		print_twotrianglegraph_from_code();
	}
	if(c == 22) {
		generate_code_twotrianglegraph();
	}
	if(c == 23) {
		isomorphism_test_operation_list_precoloredpathgraph();
	}
	if(c == 24) {
		pp_search();
	}
	if(c == 25) {
		pp_test();
	}
	if(c == 26) {
		tpp_search();
	}
	if(c == 27) {
		print_precoloredpathgraph_from_code();
	}
	if(c == 28) {
		generate_code_twoprecoloredpathgraph();
	}
	if(c == 29) {
		print_twoprecoloredpathgraph_from_code();
	}
	if(c == 30) {
		twoprecoloredpathgraph_test();
	}
	if(c == 31) {
		generate_code_precoloredpathgraph();
	}
	if(c == 32) {
		fuse_precoloredpaths_sameside();
	}
	if(c == 33) {
		fuse_twotrianglegraphs();
	}
	if(c == 34) {
		fuse_canvas_and_precoloredpathgraph_triangle_path();
	}
	if(c == 35) {
		fuse_canvas_and_precoloredpathgraph_triangle_canvas();
	}
	if(c == 36) {
		fan_length_in_endpoints();
	}
	if(c == 37) {
		restricted_precolored_path_graph_search();
	}
	if (c == 38) {
		has_large_fans_or_bellows();
	}
	if (c == 39) {
		largest_left_cut();
	}
	if (c == 40) {
		fuse_pp_and_canvas_plan2();
	}
	if (c == 41) {
		two_triangle_identification();
	}
	if (c == 42) {
		set_outer_face();
	}
}
