#include "Graph.hh"

using std::vector;
using std::map;

Graph::Graph() {
    n = 0;
    m = 0;
    al = vector<vector<int>>();
    ral = vector<map<int, int>>();
}

bool Graph::empty() const {
    return n == 0;
}