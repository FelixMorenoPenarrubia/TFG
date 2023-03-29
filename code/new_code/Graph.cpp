#include "Graph.hh"
#include "debug_utility.hh"

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

void Graph::set_starting_edge_in_al(int u, int v) {
    vector<int> newal;
    int oldidx = ral[u][v];
    int als = al[u].size();
    for (int i=0; i < als; ++i) {
        newal.push_back(al[u][(i+oldidx)%als]);
        ral[u][al[u][(i+oldidx)%als]] = i;
    }
    al[u] = newal;
}

void Graph::set_outer_face(vector<int> of) {
    int ofs = of.size();
    for (int i=0; i < ofs; ++i) {
        set_starting_edge_in_al(of[i], of[(i+1)%ofs]);
    }
}

vector<int> Graph::get_face_ccw(int u, int v) const {
    debug_assert(neighbors(u, v));

    vector<int> ans;
    ans.push_back(u);
    int nxt = v;
    int prev = u;
    while (nxt != u && (int)ans.size() < n+1) {
        ans.push_back(nxt);
        int tmp = al[nxt][(ral[nxt].at(prev)+1)%al[nxt].size()];
        prev = nxt;
        nxt = tmp;
    }
    if ((int)ans.size() == n+1) return vector<int>();
    return ans;
}