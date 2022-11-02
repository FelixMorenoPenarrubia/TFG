#include "ListGraph.hh"


using std::string;
using std::vector;

void ListGraphCode::push_r(int x) {
    code.push_back(1+x);
}

void ListGraphCode::push_f(int list_size) {
    code.push_back(F_OFFSET+list_size);
}

void ListGraphCode::push_b() {
    code.push_back(0);
}

int ListGraphCode::size() const {
    return code.size();
}

string ListGraphCode::to_string() const { 
    string s = "";
    for (int i=0; i < size(); ++i) {
        if (code[i] < 0) {
            s.push_back('F');
            int ls = code[i]-F_OFFSET;
            if (ls >= 0) {
                s.push_back('0'+ls);
            }
            else {
                s.push_back('-');
                s.push_back('0'-ls);
            }
        }
        else if (code[i] > 0) { 
            s.push_back('R');
            s.push_back('a'+code[i]-1);
        }
        else {
            s.push_back('B');
        }
    }
    return s;
}
int ListGraphCode::operator[](int i) const {
    return code[i];
}

bool ListGraphCode::operator<(const ListGraphCode& b) const {
	for (int i=0; i < std::min(size(), b.size()); ++i) {
		if (code[i] < b[i]) return true;
		if (code[i] > b[i]) return false;
	}
	return size() < b.size();
}


ListGraph::ListGraph() {
    list_sizes = vector<int>();
}


//TODO: avoid duplicating code on constructor
ListGraph::ListGraph(vector<vector<int>> _al, vector<int> _list_sizes) {
    n = _al.size();
    m = 0;
    al = _al;
    list_sizes = _list_sizes;
    generate_ral_and_m();
}

ListGraph ListGraph::remove_vertex(int w) const {
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
    return ListGraph(nal, nls);
}

ListGraph ListGraph::precolor_vertex(int w) const {
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
    return ListGraph(nal, nls);
}

bool ListGraph::nocolors() const {
    bool ans = true;
    for (int i = 0; i < n && ans; ++i) {
        if (list_sizes[i] > 0) ans = false;
    }
    return ans;
}

void ListGraph::dfs_code(int u, int idx, int& c, vector<int>& an, ListGraphCode& code) const {
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

ListGraphCode ListGraph::compute_list_code_edge(int u, int v) const {
    ListGraphCode code = ListGraphCode();
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

ListGraphCode ListGraph::compute_list_code() const {
    ListGraphCode code = ListGraphCode();

    if (n == 1) {
        code.push_f(list_sizes[0]);
        return code;
    }

    for (int u = 0; u < n; ++u) {
        for (int v : al[u]) {

            if (code.size() == 0) code = compute_list_code_edge(u, v);
            else code = std::min(code, compute_list_code_edge(u, v));
        }
    }
    return code;
}

void ListGraph::dfs_cc(int u, vector<int>& visited, vector<int>& vis_stack) const {
    if (visited[u]) return;
    visited[u] = 1;
    vis_stack.push_back(u);
    for (int v : al[u]) {
        dfs_cc(v, visited, vis_stack);
    }
}

vector<ListGraph> ListGraph::connected_components() const {
    vector<int> visited(n);
    vector<ListGraph> ans;
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

            ans.emplace_back(nal, nls);
        }
    }
    return ans;
}

//TODO: improve to linear complexity
ListGraph ListGraph::degree_assignment_subgraph() const {
    ListGraph lg = ListGraph(al, list_sizes);
    for (int u=0; u < n; ++u) {
        if ((int)al[u].size() > list_sizes[u]) {
            return lg.precolor_vertex(u).degree_assignment_subgraph();
        }
    }
    return lg;
}
