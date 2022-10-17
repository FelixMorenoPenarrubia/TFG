#include "Graph.h"
#include "CanvasToTriangles.h"
#include "AlonTarsi.h"
#include<iostream>

using std::cerr;
using std::endl;
using std::cout;
using std::cin;

void rec_alon_tarsi_test() {
    Graph g = Graph::read(cin);
    cout << recursive_alon_tarsi(g) << endl;
}

int main() {
    cerr << "Select program: " << endl;
    cerr << "1. Read canvases and print critical triangles." << endl;
    cerr << "2. Recursive Alon-Tarsi test." << endl;
    int c;
    std::cin >> c;
    if (c == 1) {
        read_canvases_and_print_critical_triangles();
    }
    if (c == 2) {
        rec_alon_tarsi_test();
    }
}