#include<iostream>
#include<fstream>
#include<vector>
#include<set>
#include<string>
#include<chrono>
#include "Canvas.hh"
#include "CanvasSearch.hh"
#include "PrecoloredPathGraph.hh"
#include "PrecoloredPathGraphSearch.hh"
#include "PrecoloredPathAndTriangleGraph.hh"
#include "GlobalSettings.hh"
#include "ReducibilityTests.hh"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::set;
using std::string;

vector<Canvas> get_canvases(int l) {
    std::fstream fin("./if/critcanvases_" + std::to_string(l) + ".txt");
    int T;
    fin >> T;
    vector<Canvas> ans;
    while(T--) {
        ans.push_back(Canvas::read(fin));
    }
    return ans;
}

vector<PrecoloredPathGraph> get_ppgs(int l) {
    std::fstream fin("./if/wed" + std::to_string(l-1) + ".txt");
    int T;
    fin >> T;
    vector<PrecoloredPathGraph> ans;
    while(T--) {
        ans.push_back(PrecoloredPathGraph::read(fin));
    }
    return ans;
}

bool test(const PrecoloredPathAndTriangleGraph& g) {
    return g.test_no_l4_adjacent_to_triangle() && g.test_criticality();
}

int main() {

    GlobalSettings::ALON_TARSI_MEMOIZE = true;

	int l;
    cin >> l;
    vector<Canvas> canvases = get_canvases(l);
    vector<PrecoloredPathGraph> ppg6 = get_ppgs(6);
    vector<PrecoloredPathGraph> ppg7 = get_ppgs(7);

    set<PrecoloredPathAndTriangleGraphCode> st;

    std::random_shuffle(canvases.begin(), canvases.end());

    auto start = std::chrono::high_resolution_clock::now();


    for (int i=0; i < (int)canvases.size(); ++i) {
        Canvas c = canvases[i];
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

        if (i%10 == 0) {
            auto now = std::chrono::high_resolution_clock::now();
            cerr << "Processed " << i+1 << "/" << canvases.size() << " canvases" << endl;
            cerr << "Seconds elapsed: " << std::chrono::duration_cast<std::chrono::seconds>(now-start).count() << endl;
        }
    }   

    cout << st.size() << endl;

    for (auto c : st) {
        PrecoloredPathAndTriangleGraph(c).write(cout);
    }

}