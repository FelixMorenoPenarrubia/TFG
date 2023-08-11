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
    vector<Canvas> ans;
    if (l <= 11) {
        std::fstream fin("./if/critcanvases_" + std::to_string(l) + ".txt");
        int T;
        fin >> T;
        
        while(T--) {
            ans.push_back(Canvas::read(fin));
        }
    }
    else {
        std::fstream fin("./if/critcanvases_" + std::to_string(l) + "_codes.txt");
        string s;
        while(fin >> s) {
            ans.push_back(Canvas(CanvasCode(s)));
        }
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

    //GlobalSettings::ALON_TARSI_MEMOIZE = true;

	int l;
    cin >> l;
    vector<Canvas> canvases = get_canvases(l);

    vector<PrecoloredPathGraph> ppg6 = get_ppgs(6);
    vector<PrecoloredPathGraph> ppg7 = get_ppgs(7);

    set<PrecoloredPathAndTriangleGraphCode> st;

    auto start = std::chrono::high_resolution_clock::now();

    vector<CanvasWithIndices> c6;
    vector<CanvasWithIndices> c7;

    for (int i=0; i < (int)canvases.size(); ++i) {
        Canvas c = canvases[i];
        auto c6a = PrecoloredPathAndTriangleGraph::critical_canvases_with_indices_triangle_in_canvas(c);
        auto c7a = PrecoloredPathAndTriangleGraph::critical_canvases_with_indices_triangle_in_path(c);
        for (auto c2 : c6a) {
            c2.c_idx = i;
            c6.push_back(c2);
        }
        for (auto c2 : c7a) {
            c2.c_idx = i;
            c7.push_back(c2);
        }
    }

    auto now = std::chrono::high_resolution_clock::now();
    cerr << "Processed critical indices canvases, c6: " << c6.size() << " c7: " << c7.size() << endl;
    cerr << "Seconds elapsed: " << std::chrono::duration_cast<std::chrono::seconds>(now-start).count() << endl;


    for (int i=0; i < (int)c6.size(); ++i) {
        CanvasWithIndices cwi = c6[i];
        for (PrecoloredPathGraph ppg : ppg6) {
            auto vec = PrecoloredPathAndTriangleGraph::fuse_canvas_with_indices_and_path_triangle_in_canvas(canvases[cwi.c_idx], cwi, ppg);
            for (auto g : vec) {
                if (st.find(g.compute_code()) == st.end() && test(g)) {
                    st.insert(g.compute_code());
                }
            }
        }

        if (i%100 == 0) {
            now = std::chrono::high_resolution_clock::now();
            cerr << "Processed " << i+1 << "/" << c6.size() << " c6 canvases" << endl;
            cerr << "Seconds elapsed: " << std::chrono::duration_cast<std::chrono::seconds>(now-start).count() << endl;
        }
    }

    for (int i=0; i < (int)c7.size(); ++i) {
        CanvasWithIndices cwi = c7[i];
        for (PrecoloredPathGraph ppg : ppg7) {
            auto vec = PrecoloredPathAndTriangleGraph::fuse_canvas_with_indices_and_path_triangle_in_path(canvases[cwi.c_idx], cwi, ppg);
            for (auto g : vec) {
                if (st.find(g.compute_code()) == st.end() && test(g)) {
                    st.insert(g.compute_code());
                }
            }
        }

        if (i%100 == 0) {
            now = std::chrono::high_resolution_clock::now();
            cerr << "Processed " << i+1 << "/" << c7.size() << " c7 canvases" << endl;
            cerr << "Seconds elapsed: " << std::chrono::duration_cast<std::chrono::seconds>(now-start).count() << endl;
        }
    }      

    cout << st.size() << endl;

    for (auto c : st) {
        PrecoloredPathAndTriangleGraph(c).write(cout);
    }

}