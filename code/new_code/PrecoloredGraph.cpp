#include "PrecoloredGraph.hh"
#include "debug_utility.hh"

PrecoloredGraph::PrecoloredGraph() {
    precolored = vector<int>();
}

ListGraph PrecoloredGraph::compute_list_graph() const {
    int nn = 0;
    vector<int> morph(n);
    for (int i=0; i < n; ++i) {
        if (!precolored[i]) {
            morph[i] = nn++;
        }
    }

    //debug_var(nn);

    vector<int> nls (nn);
    vector<vector<int>> nal (nn);

    for (int u=0; u < n; ++u) {
        if (!precolored[u]) {
            nls[morph[u]] = list_sizes[u];
            for (int v : al[u]) {
                if (!precolored[v]) {
                    nal[morph[u]].push_back(morph[v]);
                }
                else {
                    nls[morph[u]]--;
                }
            }
        }
    }

    return ListGraph(nal, nls);
}