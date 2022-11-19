#include "TwoTriangleGraph.hh"
#include "ReducibilityTests.hh"

#include<iostream>
#include<algorithm>
#include<queue>

using std::vector;
using std::endl;
using std::string;

//TODO: Eliminate code duplication, make a general code
//for all precolored graphs

TwoTriangleGraphCode::TwoTriangleGraphCode() {
	
}
	
void TwoTriangleGraphCode::push(int x) {
    code.push_back(x);
}

void TwoTriangleGraphCode::push_f(bool precolored) {
    if(!precolored) code.push_back(-1);
    else code.push_back(-3);
}

void TwoTriangleGraphCode::push_b() {
    code.push_back(-2);
}

int TwoTriangleGraphCode::size() const {
    return code.size();
}

string TwoTriangleGraphCode::to_string() const { //can be made better
    string s = "";
    for (int i=0; i < size(); ++i) {
        if (code[i] == -1) s.push_back('F');
        else if (code[i] == -2) s.push_back('B');
        else if (code[i] == -3) s.push_back('P');
        else if (code[i] <= 9) s.push_back('0'+code[i]);
        else s.push_back('a'+code[i]-10);
    }
    return s;
}

int TwoTriangleGraphCode::operator[](int i) const {
    return code[i];
}

TwoTriangleGraphCode::TwoTriangleGraphCode(const string& s) {
    for (int i=0; i < (int)s.length(); ++i) {
        if (s[i] == 'F') code.push_back(-1);
        else if (s[i] == 'B') code.push_back(-2);
        else if (s[i] == 'P') code.push_back(-3);
        else if ('0' <= s[i] && s[i] <= '9') code.push_back(s[i]-'0');
        else code.push_back(s[i]-'a'+10);
    }
}

bool TwoTriangleGraphCode::operator<(const TwoTriangleGraphCode& b) const {
	for (int i=0; i < std::min((int)code.size(), b.size()); ++i) {
		if (code[i] < b[i]) return true;
		if (code[i] > b[i]) return false;
	}
	return (int)code.size() < b.size();
}


//TODO: revise empty constructors, any way to avoid them?
TwoTriangleGraph::TwoTriangleGraph() {
    n = 0;
    path_length = 0;
}

TwoTriangleGraph::TwoTriangleGraph(vector<vector<int>> _al, vector<vector<int>> _triangles) {

    n = _al.size();
    al = _al;
    triangles = _triangles;
    precolored = vector<int>(n);
    for (int u : triangles[0]) precolored[u] = 1;
    for (int u : triangles[1]) precolored[u] = 1;

    generate_ral_and_m();  
    set_list_sizes();

    path_length = distance_between_triangles();
}

TwoTriangleGraph::TwoTriangleGraph(const Canvas& g, int s) {
    original_canvas = g;

    path_length = (g.l-6)/2;
    
    n = g.n-(path_length+1);
    al = vector<vector<int> >(n);


    vector<int> morph(g.n);
    vector<int> corr(g.n, -1);

    for (int i=0; i < path_length+1; ++i) {
        morph[(s+i)%g.l] = i;
        morph[(s+2*path_length+3-i)%g.l] = i;
        corr[((s+2*path_length+3-i)%g.l)] = (s+i)%g.l;
    }
    morph[(s+path_length+1)%g.l] = path_length+1;
    morph[(s+path_length+2)%g.l] = path_length+2;
    morph[(s-2+g.l)%g.l] = path_length+3;
    morph[(s-1+g.l)%g.l] = path_length+4;
    for (int i=g.l; i < g.n; ++i) {
        morph[i] = i-g.l+path_length+5;
    }

    precolored = vector<int>(n);
    precolored[0] = 1;
    precolored[path_length] = 1;
    precolored[path_length+1] = 1;
    precolored[path_length+2] = 1;
    precolored[path_length+3] = 1;
    precolored[path_length+4] = 1;

    triangles.push_back({0, path_length+3, path_length+4});
    triangles.push_back({path_length, path_length+1, path_length+2});

    vector<vector<int>> am(g.n, vector<int>(g.n));

    for (int u = 0; u < g.n; ++u) {
        for (int v : g.al[u]) {
            am[u][v] = 1;
        }
    }

    for (int i = 0; i < path_length+1; ++i) {
        int u = (s+i)%g.l;
        int nu = (s+i+1)%g.l;
        int us = g.al[u].size();
        for (int j=0;  j < us; ++j) {
            int v = g.al[u][(j+g.ral[u].at(nu))%us];
            al[morph[u]].push_back(morph[v]); //TODO: Assuming no chords
        }
        std::reverse(al[morph[u]].begin(), al[morph[u]].end());
        int cu = (s+2*path_length+3-i)%g.l;
        int cnu = (cu+1)%g.l;
        int cus = g.al[cu].size();
        vector<int> nalu;
        for (int j=(i==0 ? 0 : 1);  j+(i==path_length ? 0 : 1) < cus; ++j) {
            int v = g.al[cu][(j+g.ral[cu].at(cnu))%cus];
            if (!am[u][v]) nalu.push_back(morph[v]); 
        }
        reverse(nalu.begin(), nalu.end());
        for (int x : nalu) al[morph[u]].push_back(x);

    }

    for (int u = 0; u < g.l; ++u) {
        if (morph[u] == -1 || !precolored[morph[u]] || morph[u] == 0 || morph[u] == path_length) continue;
         for (int j = (int)g.al[u].size()-1; j > -1; --j) {
            int v = g.al[u][j];
            if (corr[v] != -1 && am[u][corr[v]]) continue;
            al[morph[u]].push_back(morph[v]);
        }
    }

    for (int u=g.l; u < g.n; ++u) {
        for (int j = (int)g.al[u].size()-1; j > -1; --j) {
            int v = g.al[u][j];
            if (corr[v] != -1 && am[u][corr[v]]) continue;
            al[morph[u]].push_back(morph[v]);
        }
    }

    generate_ral_and_m();  
    set_list_sizes();
}

TwoTriangleGraph TwoTriangleGraph::read(std::istream& is) {
    int n, m, path_length;
    is >> n >> m >> path_length;
    vector<vector<int> > triangles (2, vector<int>(3));
    is >> triangles[0][0] >> triangles[0][1] >> triangles[0][2] >> triangles[1][0] >> triangles[1][1] >> triangles[1][2];
    vector<vector<int>> al(n);
    for (int i=0; i < 2*m; ++i) {
        int u, v;
        is >> u >> v;
        al[u].push_back(v);
    }
    return TwoTriangleGraph(al, triangles);
}


void TwoTriangleGraph::write(std::ostream& os) const {
    os << n << " " << m << " " << path_length << endl;
    for (int i=0; i < n; ++i) {
        if (precolored[i]) os << i << " ";
    }
    os << endl;
    for (int u = 0; u < n; ++u) {
        for (int v : al[u]) {
            os << u << " " << v << endl;
        }
    }
}

//TODO: only works for d = 1 (precolored vertices are prefix)
void TwoTriangleGraph::write_prolog(std::ostream& os) const {
    os << "numVertices(" << n << ")." << endl;
    os << "numVerticesTSubgraph(6)." << endl;
    for (int i=0; i < n; ++i) {
        for (int j : al[i]) {
            if (i < j) {
                if(precolored[i] && precolored[j]) {
                    os << "tEdge(" << i << "," << j << ")." << endl;
                }
            }
        }
    }
    for (int i=0; i < n; ++i) {
        for (int j : al[i]) {
            if (i < j) {
                if(!(precolored[i] && precolored[j])) {
                    os << "iEdge(" << i << "," << j << ")." << endl;
                }
            }
        }
    }
    
}

vector<TwoTriangleGraph> TwoTriangleGraph::generate_from_canvas(const Canvas& g) {
    vector<TwoTriangleGraph> ans;
    for (int i=0; i < g.l; ++i) {
        ans.push_back(TwoTriangleGraph(g, i));
    }
    return ans;
}

void TwoTriangleGraph::dfs_code(int u, int idx, int& c, vector<int>& an, TwoTriangleGraphCode& code) const {
    if (an[u] != -1) {
        code.push(an[u]);
        return;
    }
    code.push_f(precolored[u]);
    an[u] = c++;
    int als = (int)al[u].size();
    for (int i=1; i < als; ++i) {
        int v = al[u][(i+idx)%als];
        dfs_code(v, ral[v].at(u), c, an, code);
    }
    code.push_b();
}

TwoTriangleGraphCode TwoTriangleGraph::compute_code_edge(int u, int v) const {
    TwoTriangleGraphCode code = TwoTriangleGraphCode();
    vector<int> assigned_numbers (n, -1);
    assigned_numbers[u] = 0;
    int c = 1;
    dfs_code(v, ral[v].at(u), c, assigned_numbers, code);
    return code;
}

//Note: may give different codes for isomorphic graphs if the outer orientation is reversed 
TwoTriangleGraphCode TwoTriangleGraph::compute_code() const {
    TwoTriangleGraphCode code;

    for (int u=0; u < n; ++u) {
        if (precolored[u]) {
            for (int v = u+1; v < n; ++v) {
                if (precolored[v] && ral[v].find(u) != ral[v].end()) {
                    if (code.size() == 0) code = std::min(compute_code_edge(u, v), compute_code_edge(v, u));
                    else code = std::min(code, std::min(compute_code_edge(u, v), compute_code_edge(v, u)));
                }
            }
        }
    }
    
    return code;
}

void TwoTriangleGraph::set_list_sizes() {
    list_sizes = vector<int>(n, 5);
    for (int i=0; i < n; ++i) {
        if (precolored[i]) list_sizes[i] = 1;
    }
}

int TwoTriangleGraph::distance_between_triangles() const {
    vector<int> dist(n, -1);
    std::queue<int> q;
    for (int u : triangles[0]) {
        dist[u] = 0;
        q.push(u);
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : al[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u]+1;
                q.push(v);
            }
        }
    }
    int ans = dist[triangles[1][0]];
    for (int u : triangles[1]) {
        ans = std::min(ans, dist[u]);
    }
    return ans;
}

bool TwoTriangleGraph::test_criticality() const {
    if (n == 6) return true;
    if (distance_between_triangles() < path_length) return false;
    return !batch_test(compute_list_graph());
}