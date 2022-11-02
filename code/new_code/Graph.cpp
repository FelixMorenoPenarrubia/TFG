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

bool Graph::neighbors(int u, int v) const {
    return ral[u].find(v) != ral[u].end();
}

void Graph::generate_ral_and_m() {
    ral = vector<std::map<int, int>>(n);
    for (int u=0; u < n; ++u) {
        for (int j=0; j < (int)al[u].size(); ++j) {
            int v = al[u][j];
            ral[u][v] = j;
            m++;
        }
    }
    m /= 2;
}