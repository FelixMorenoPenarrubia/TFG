#include<iostream>
#include<vector>
#include<set>
#include<string>
#include "Canvas.hh"
#include "CanvasSearch.hh"
#include "PrecoloredPathGraph.hh"
#include "PrecoloredPathGraphSearch.hh"
#include "PrecoloredPathAndTriangleGraph.hh"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::set;
using std::string;


bool test(const PrecoloredPathAndTriangleGraph& g) {
    return g.test_no_l3_adjacent() && g.test_no_l4_adjacent_to_triangle() && g.test_criticality();
}

int main() {
    CanvasSearch cs;
    PrecoloredPathGraphSearch ppgs;
	int l;
    cin >> l;
    vector<Canvas> canvases = cs.get_chordless(l);
    vector<PrecoloredPathGraph> ppg6 = ppgs.get_all(6);
    vector<PrecoloredPathGraph> ppg7 = ppgs.get_all(7);

    set<PrecoloredPathAndTriangleGraphCode> st;

    for (Canvas c : canvases) {
        for (PrecoloredPathGraph ppg : ppg6) {
            auto vec = PrecoloredPathAndTriangleGraph::fuse_canvas_and_path_triangle_in_canvas(c, ppg);
            for (auto g : vec) {
                if (st.find(g.compute_code()) == st.end() && test(g)) {
                    st.insert(g.compute_code());
                }
            }
        }
        for (PrecoloredPathGraph ppg : ppg7) {
            auto vec = PrecoloredPathAndTriangleGraph::fuse_canvas_and_path_triangle_in_path(c, ppg);
            for (auto g : vec) {
                if (st.find(g.compute_code()) == st.end() && test(g)) {
                    st.insert(g.compute_code());
                }
            }
        }
    }   

    cout << st.size() << endl;

    for (auto c : st) {
        PrecoloredPathAndTriangleGraph(c).write(cout);
    }

}