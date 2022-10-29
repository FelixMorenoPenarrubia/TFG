#include "Graph.h"
#include "AlonTarsi.h"
#include<iostream>
#include<vector>
#ifndef CANVASTOTRIANGLES_H_
#define CANVASTOTRIANGLES_H_

using std::vector;
struct TwoTriangleGraph {
    Graph g;
    int path_length;
    vector<vector<int> > original_canvas;
    vector<vector<int> > edges_to_triangles;
    TwoTriangleGraph();
    TwoTriangleGraph(Graph _g, int _pl, vector<vector<int>> _oc, vector<vector<int>> _ett);
    void write_verbose (std::ostream &os);
    void write_prolog (std::ostream &os);
    void write (std::ostream &os);
};

void process_canvas();
void read_canvases_and_print_critical_triangles();

#endif

