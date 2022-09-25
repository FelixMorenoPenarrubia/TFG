#include<iostream>
#include<vector>
#include<set>
#include<queue>
#include<functional>
#include<map>
#include "PlaneGraph.cpp"

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::set;
using std::queue;
using std::cerr;
using pg = PlaneGraph;
using ll = long long;


vector<vector<pg>> canvases;
set<Code> canvases_codes;
std::map<Code, ll> profile_by_code;

const bool PRINT_CANVAS_ON_ADD = false;
const bool PRINT_CANVAS_ON_END = true;
const bool PAUSE_ON_ADD = false;
const bool TEST_STATISTICS = false;
const bool SIZE_STATISTICS = false;
const bool ADD_CHORDS = false; //TODO: fix bugs, do not use
const bool DO_NOT_STORE_LAST = false;
const bool REPORT_QUEUE_SIZE = false;
const bool PROLOG_OUTPUT_FORMAT = true;

void print_canvas(pg g) {
	cout << "-------" << endl;
	cout << "n = " << g.n << ", m = " << g.m << ", l = " << g.l << endl;
	cout << "Adjacency list:" << endl;
	for(int u=0; u < g.n; ++u) {
		for(int v : g.al[u]) {
			cout << u << " " << v << endl;	
		}
	}
	if(PROLOG_OUTPUT_FORMAT) {
		cout << "------" << endl;
		cout << "interiorListSize(5)." << endl;
		cout << "numColors(6)." << endl;
		cout << "sizeOuterCycle(" << g.l << ")." << endl;
		cout << "numVertices(" << g.n << ")." << endl;
		for(int u=0; u < g.n; ++u) {
			for(int v : g.al[u]) {
				if (u < v) cout << "edge(" << u << "," << v << ")." << endl;	
			}
		}
	}
	cout << "-----" << endl;
	cout << "Code: " << g.compute_code().to_string() << endl;
	cout << "-------" << endl;
}



bool test_canvas(pg g) {
	vector<std::function<bool()>> tests = {std::bind(&PlaneGraph::degree_test, g),
	 									   std::bind(&PlaneGraph::deficiency_test, g),
										   std::bind(&PlaneGraph::gadget4_test, g),
										   std::bind(&PlaneGraph::biconnected_deg5_components_test, g)
										  /* std::bind(&PlaneGraph::gadget5_test, g)*/};
	/*
	if(!g.degree_test()) return false;
	if(!g.deficiency_test()) return false;
	if(!g.gadget4_test()) return false;
	if(!g.biconnected_deg5_components_test()) return false;
	if(!g.gadget5_test()) return false;
	*/

	if(TEST_STATISTICS) {
		ll test_profile = 0;
		for (int i=0; i < (int)tests.size(); ++i) {
			if(tests[i]()) test_profile |= (1LL<<i);
		}
		profile_by_code[g.compute_code()] = test_profile;
		if(test_profile != (1LL<<tests.size())-1) return false;
	}
	else {
		for(auto f : tests) {
			if(!f()) return false;
		}
	}

	return true;
}



void pause() {
	cin.get();
}

bool add_canvas(int l, pg g) {
	if(!test_canvas(g)) return false;
	Code c = g.compute_code();
	if(canvases_codes.find(c) == canvases_codes.end()) {
		if(!DO_NOT_STORE_LAST || l+1 < (int) canvases.size()) {
			canvases[l].push_back(g);
		}
		else {
			canvases[l].push_back(pg(0, vector<vector<int>>())); //TODO: this is a bad patch
		}

		canvases_codes.insert(c);
		if(PRINT_CANVAS_ON_ADD) print_canvas(g);
		if(PAUSE_ON_ADD) pause();
		
		return true;
	}
	return false;
}

pg empty_cycle(int l) {
	vector<vector<int>> al(l);
	for(int i=0; i < l; ++i) {
		al[i].push_back((i+1)%l);
		al[i].push_back((i+l-1)%l);
	}
	return pg(l, al);
}

void gen(int l) {
	queue<pg> q;

	//Empty cycle
	add_canvas(l, empty_cycle(l));
	q.push(empty_cycle(l));
	
	//Add chords
	if(ADD_CHORDS) {
		//TODO: fix bugs
		for(int a=3; a < l; ++a) {
			int b = l-a+2;
			for(pg g1 : canvases[a]) {
				for(pg g2 : canvases[b]) {
					for(int j1=0; j1 < a; ++j1) {
						for(int j2=0; j2 < b; ++j2) {
							pg ng1 = PlaneGraph::fuse_chord(g1, g2, j1, j2, false);
							pg ng2 = PlaneGraph::fuse_chord(g1, g2, j1, j2, true);
							if(add_canvas(l, ng1)) q.push(ng1);
							if(add_canvas(l, ng2)) q.push(ng2);
						}
					}
				}
			}
		}
	}
	
	//Add tripods from smaller sizes
	for(int k=3; k < l; ++k) {  //for each canvas size up to l-1
		ll ts = l-k+1;
		for(pg g : canvases[k]) { //for each canvas of that size
			for(int j=0; j < k; ++j) { //for each outer vertex of the canvas
				for(ll bm=1; bm < (1LL<<ts); ++bm) { //for each disposition of the chords of the tripod (bitmask)
					pg ng = g.add_tripod(ts, j, bm);
					if(add_canvas(l, ng)) q.push(ng);
				}
			}
		}
	}
	
	//Add tripods from the same size
	while(!q.empty()) {
		pg g = q.front();
		q.pop();
		for(int j=0; j < l; ++j) { //for each outer vertex of the canvas
			pg ng = g.add_tripod(1, j, 1);
			if(add_canvas(l, ng)) q.push(ng);

			if(REPORT_QUEUE_SIZE && rand()%10000 == 0 && !q.empty()) {
				cout << "Queue size: " << q.size() << endl;
				cout << "Current canvas n: " << g.n << endl;
			}

		}
	}
}

void search() {
	cout << "Enter maximum l:" << endl;
	int l;
	cin >> l;
	canvases = vector<vector<pg>>(l+1);
	for(int i=3; i <= l; ++i) {
		gen(i);
		cout << ".-.-.-.-.-.-.-.-." << endl;
		cout << "Canvases of size " << i << " (total: " << canvases[i].size() << ")" << endl;
		cout << ".-.-.-.-.-.-.-.-." << endl;
		cout << endl << endl;
		if(PRINT_CANVAS_ON_END) {
			for (pg g : canvases[i]) {
				print_canvas(g);
			}
			cout << endl << endl;
		}
		if(TEST_STATISTICS) {

			std::map<int, int> stats;
			for (auto p : profile_by_code) {
				stats[p.second]++;
			}
			profile_by_code.clear();

			cout << "---" << endl;
			cout << "Test statistics:" << endl;
			for (auto p : stats) {
				cout << p.first << ": " << p.second << endl;
			}
			cout << endl;
		}

		if(SIZE_STATISTICS) {
			std::map<int, int> stats;
			for(pg g : canvases[i]) {
				stats[g.n]++;
			}

			cout << "---" << endl;
			cout << "Size statistics:" << endl;
			for (auto p : stats) {
				cout << p.first << ": " << p.second << endl;
			}
			cout << endl;

		}
	}
}
