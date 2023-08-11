#include<iostream>
#include<vector>
#include<string>
#include<set>
#include<queue>
#include "PrecoloredPathGraph.hh"
#include "CanvasSearch.hh"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

std::set<PrecoloredPathGraphCode> s;
std::vector<PrecoloredPathGraphCode> st;

int max_mn = 0;

void add_graph(const PrecoloredPathGraph& g) {
    if (s.find(g.compute_code()) == s.end()) {
        cerr << "Testing graph:" << endl;
        cerr << g.compute_code().to_string() << endl;
        if (g.test_criticality()) {
            s.insert(g.compute_code());
            st.push_back(g.compute_code());
            if (g.n + g.m > max_mn) {
                PrecoloredPathGraph(g.compute_code()).write(cerr);
                max_mn = g.n+g.m;
            }
        }
    }
}

int main() {
    
	
    int T;
    cin >> T;
    for (int t=0; t < T; ++t) {
        //add_graph(PrecoloredPathGraph::read(cin));
        
        st.push_back(PrecoloredPathGraph::read(cin).compute_code());
        s.insert(st.back());
    }

    cerr << "Finished reading input" << endl;
    CanvasSearch cs;
    vector<Canvas> vc = cs.get_chordless(6);
    
    while(!st.empty()) {
        PrecoloredPathGraph g = PrecoloredPathGraph(st.back());
        st.pop_back();
       // cerr << "Fusing with graph: " << g.compute_code().to_string() << endl;

        for (Canvas c : vc) {
           // cerr << "Fusing with canvas " << c.compute_code().to_string() << endl;
            vector<PrecoloredPathGraph> v = g.fuse_canvas(c);
            for (auto ng : v) {
                add_graph(ng);
            }
        }
        
            cerr << "Stack size = " << st.size() << endl;
        
    }

	
	for (auto c : s) {
		PrecoloredPathGraph(c).write(cout);
	}
}