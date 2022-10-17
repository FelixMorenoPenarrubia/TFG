#include "Graph.h"
#include "debug_utility.h"

using std::endl;



Graph::Graph() {
    n = 0;
    m = 0;
}

Graph::Graph(vector<int> _list_sizes, vector<vector<int>> _al) {
    list_sizes = _list_sizes;
    al = _al;
    n = list_sizes.size();
    m = 0;
    ral = vector<std::map<int, int>> (n);
    for (int u = 0; u < n; ++u) {
        for (int j = 0; j < (int) al[u].size(); ++j) {
            int v = al[u][j];
            ral[u][v] = j;
            m++;
        }
    }
    m /= 2;
}

bool Graph::empty() const {
    return n == 0;
}

bool Graph::nocolors() const {
    bool ans = true;
    for (int i = 0; i < n && ans; ++i) {
        if (list_sizes[i] > 0) ans = false;
    }
    return ans;
}

Graph Graph::read(std::istream& is) {
    int n, m;
    is >> n >> m;
    vector<int> list_sizes(n);
    for (int u = 0; u < n; ++u) {
        is >> list_sizes[u];
    }
    vector<vector<int> > al (n);
    for (int e = 0; e < 2*m; ++e) {
        int u, v;
        is >> u >> v;
        al[u].push_back(v);
    }
    return Graph(list_sizes, al);
}

void Graph::write(std::ostream& os) const {
    os << n << " " << m << endl;
    for (int u = 0; u < n; ++u) {
        os << list_sizes[u] << " ";
    }
    os << endl;
    for (int u = 0; u < n; ++u) {
        for (int v : al[u]) {
            os << u << " " << v << endl;
        }
    }
}

void Graph::dfs_code(int u, int idx, int& c, vector<int>& an, GraphCode& code) const {
    if (an[u] != -1) {
        code.push_r(an[u]);
        return;
    }

    code.push_f(list_sizes[u]);
    an[u] = c++;
    int als = (int)al[u].size();
    for (int i=1; i < als; ++i) {
        int v = al[u][(i+idx)%als];
        dfs_code(v, ral[v].at(u), c, an, code);
    }
    code.push_b();
}

GraphCode Graph::compute_code_edge(int u, int v) const {
    GraphCode code = GraphCode();
    vector<int> assigned_numbers (n, -1);
    assigned_numbers[u] = 0;
    code.push_f(list_sizes[u]);
    int c = 1;
    for (int i = 0; i < (int)al[u].size(); ++i) {
        int w = al[u][(i+ral[u].at(v))%al[u].size()];
        dfs_code(w, ral[w].at(u), c, assigned_numbers, code);
    }
    return code;
}

GraphCode Graph::compute_code() const {
    GraphCode code = GraphCode();
    for (int u = 0; u < n; ++u) {
        for (int v : al[u]) {

            if (code.size() == 0) code = compute_code_edge(u, v);
            else code = std::min(code, compute_code_edge(u, v));
        }
    }
    return code;
}


//TODO: remove code duplication?
Graph Graph::remove_vertex(int w) const {
    vector<int> morph(n);
    for (int u = 0; u < w; ++u) {
        morph[u] = u;
    }
    for (int u = w+1; u < n; ++u) {
        morph[u] = u-1;
    }
    vector<int> nls (n-1, 0);
    vector< vector<int> > nal (n-1);
    for (int u = 0; u < n; ++u) {
        if (u != w) {
            nls[morph[u]] += list_sizes[u];
            for (int v : al[u]) {
                if (v != w) {
                    nal[morph[u]].push_back(morph[v]);
                }
            }
        }
    }
    return Graph(nls, nal);
}

Graph Graph::precolor_vertex(int w) const {
    vector<int> morph(n);
    for (int u = 0; u < w; ++u) {
        morph[u] = u;
    }
    for (int u = w+1; u < n; ++u) {
        morph[u] = u-1;
    }
    vector<int> nls (n-1, 0);
    vector< vector<int> > nal (n-1);
    for (int u = 0; u < n; ++u) {
        if (u == w) {
            for (int v : al[u]) {
                nls[morph[v]]--;
            }
        }
        else {
            nls[morph[u]] += list_sizes[u];
            for (int v : al[u]) {
                if (v != w) {
                    nal[morph[u]].push_back(morph[v]);
                }
            }
        }
    }
    return Graph(nls, nal);
}

void Graph::dfs_cc(int u, vector<int>& visited, vector<int>& vis_stack) const {
    if (visited[u]) return;
    visited[u] = 1;
    vis_stack.push_back(u);
    for (int v : al[u]) {
        dfs_cc(v, visited, vis_stack);
    }
}

vector<Graph> Graph::connected_components() const {
    vector<int> visited(n);
    vector<Graph> ans;
    for (int u = 0; u < n; ++u) {
        if (!visited[u]) {
            vector<int> vis_st;
            dfs_cc(u, visited, vis_st);
            int nn = vis_st.size();
            std::map<int, int> morph;
            vector<vector<int> > nal (nn);
            vector<int> nls (nn);

            for (int i=0; i < nn; ++i) {
                morph[vis_st[i]] = i;
            }

            for (int i=0; i < nn; ++i) {
                nls[i] = list_sizes[vis_st[i]];
                for (int v : al[vis_st[i]]) {
                    nal[i].push_back(morph[v]);
                }
            }

            ans.emplace_back(nls, nal);
        }
    }
    return ans;
}