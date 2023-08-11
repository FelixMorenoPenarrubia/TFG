#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<chrono>
#include "Canvas.hh"
#include "TwoTriangleGraph.hh"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

void process_canvas(CanvasCode cc) {
    
    Canvas c = Canvas(cc);
    vector<TwoTriangleGraph> ttg = TwoTriangleGraph::generate_from_canvas(c);
    for (auto g : ttg) {
        if (g.test_criticality()) {
            cout << g.compute_code().to_string() << endl;
            //cerr << "CRITICAL GRAPH FOUND !!!!!!!! INFORM FÉLIX" << endl;
        }
    }
    
}

int main() {
    //int T;
    //cin >> T;
   // T = 1e6;
   string str;
	while (cin >> str) {
        process_canvas(CanvasCode(str));
        /*if (t%(T/20) == 0) {
                cerr << 5*(t/(T/20)) << "% " << std::flush;
        }*/
    }
    //cerr << endl;
}