#include "TwoPrecoloredPathsGraph.hh"
#include "ReducibilityTests.hh"
#include "debug_utility.hh"

using ll = long long;
using std::string;
using std::endl;

TwoPrecoloredPathsGraphCode::TwoPrecoloredPathsGraphCode() {
	
}
	
void TwoPrecoloredPathsGraphCode::push_r(int x) {
    code.push_back(1+x);
}

void TwoPrecoloredPathsGraphCode::push_f(int list_size) {
    code.push_back(F_OFFSET+list_size);
}

void TwoPrecoloredPathsGraphCode::push_b() {
    code.push_back(0);
}

int TwoPrecoloredPathsGraphCode::size() const {
    return code.size();
}

string TwoPrecoloredPathsGraphCode::to_string() const { //can be made better
    string s = "";
    for (int i=0; i < size(); ++i) {
        if (code[i] < 0) {
            s.push_back('F');
            int ls = code[i]-F_OFFSET;
            //Assuming ls >= 0
            s.push_back('0'+ls);
           
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

TwoPrecoloredPathsGraphCode::TwoPrecoloredPathsGraphCode(const string& s) {
    for (int i=0; i < (int)s.length(); ++i) {
        if (s[i] == 'F') {
            //assuming 0 <= ls <= 9
        }
        else if (s[i] == 'R') {

        }
        else if (s[i] == 'B') {
            code.push_back(0);
        }
        else if ('0' <= s[i] && s[i] <= '9') {
             code.push_back((s[i]-'0')+F_OFFSET);
        }
        else {
            code.push_back(s[i]-'a'+1);
        }
    }
}


int TwoPrecoloredPathsGraphCode::operator[](int i) const {
    return code[i];
}


bool TwoPrecoloredPathsGraphCode::operator<(const TwoPrecoloredPathsGraphCode& b) const {
	for (int i=0; i < std::min((int)code.size(), b.size()); ++i) {
		if (code[i] < b[i]) return true;
		if (code[i] > b[i]) return false;
	}
	return (int)code.size() < b.size();
}


TwoPrecoloredPathsGraph::TwoPrecoloredPathsGraph() {

}

TwoPrecoloredPathsGraph::TwoPrecoloredPathsGraph(vector<vector<int>> _al, vector<int> _list_sizes) {
    n = _al.size();
    list_sizes = _list_sizes;
    m = 0;
    al = _al;
    generate_ral_and_m();
    init_inherited_values();
}

void TwoPrecoloredPathsGraph::init_inherited_values() {
    precolored = vector<int>(n, 0);
    for (int i=0; i < n; ++i) {
        if (list_sizes[i] == 1) {
            precolored[i] = 1;
        }
    }
}

void TwoPrecoloredPathsGraph::write(std::ostream& os) const {
    os << n << " " << m << endl;
    for (int u = 0; u < n; ++u) {
        os << list_sizes[u];
        if (u+1 < n) os << " ";
    }
    os << endl;

    for (int u = 0; u < n; ++u) {
        for (int v : al[u]) {
            os << u << " " << v << endl;
        }
    }
}

void TwoPrecoloredPathsGraph::write_prolog(std::ostream& os) const {
    os << "numVertices(" << n << ")." << endl;
    os << "numVerticesTSubgraph(" << 4 << ")." << endl;
    for (int i=0; i < n; ++i) {
        os << "listSize(" << i << "," << list_sizes[i] << ")." << endl;
    }
    for (int i=0; i < n; ++i) {
        for (int j : al[i]) {
            if (i < j) {
                if(precolored[i] && precolored[j]) {
                    os << "tEdge(" << i << "," << j << ")." << endl;
                }
            }
        }
    }

    bool hasIEdges = false;
    for (int i=0; i < n; ++i) {
        for (int j : al[i]) {
            if (i < j) {
                if(!(precolored[i] && precolored[j])) {
                    os << "iEdge(" << i << "," << j << ")." << endl;
                    hasIEdges = true;
                }
            }
        }
    }

    //TODO: this is a trashy fix
    if (!hasIEdges) {
        os << "iEdge(-1, -1)." << endl;
    }
    
}


TwoPrecoloredPathsGraph TwoPrecoloredPathsGraph::read(std::istream& is) {
    int n, m;
	is >> n >> m;
    vector<int> list_sizes(n);
    for (int i=0; i < n; ++i) is >> list_sizes[i];
	vector<vector<int> > al(n);
	for (int i=0; i < 2*m; ++i) {
		int u, v;
		is >> u >> v;
		al[u].push_back(v);
	}
	return TwoPrecoloredPathsGraph(al, list_sizes);
}


TwoPrecoloredPathsGraph TwoPrecoloredPathsGraph::read_code(std::istream& is) {
    string s;
    is >> s;
    return TwoPrecoloredPathsGraph(TwoPrecoloredPathsGraphCode(s));
}


TwoPrecoloredPathsGraph TwoPrecoloredPathsGraph::fuse_precoloredpaths_sameside(const PrecoloredPathGraph& g1, const PrecoloredPathGraph& g2) {
    debug_assert(g1.l == g2.l+2);
    vector<vector<int>> nal (g1.n + g2.n - g2.l);
    vector<int> nlistsizes(g1.n + g2.n - g2.l);
    vector<int> morph1(g1.n);
    vector<int> morph2(g2.n);

    for (int i=0; i < g1.n; ++i) {
        morph1[i] = i;
    }
    for (int i=0; i < g2.l; ++i) {
        morph2[i] = g2.l-i;
    }
    for (int i=g2.l; i < g2.n; ++i) {
        morph2[i] = i+g1.n-g2.l;
    }

    for (int u = 0; u < g1.n; ++u) {
        nlistsizes[morph1[u]] = g1.list_sizes[u];
        for (int v : g1.al[u]) {
            nal[morph1[u]].push_back(morph1[v]);
        }
    }

    for (int u = 0; u < g2.n; ++u) {
        nlistsizes[morph2[u]] = g2.list_sizes[u];
        for (int v : g2.al[u]) {
            if (u < g2.l && v < g2.l) continue;
            nal[morph2[u]].push_back(morph2[v]);
        }
    }

    for (int i=2; i < g2.l; ++i) {
        nlistsizes[i] = 5;
    }

    return TwoPrecoloredPathsGraph(nal, nlistsizes);
}

TwoPrecoloredPathsGraph TwoPrecoloredPathsGraph::fuse_precoloredpaths_oppositeside(const PrecoloredPathGraph& g1, const PrecoloredPathGraph& g2) {
    debug_assert(g1.l == g2.l);
    vector<vector<int>> nal (g1.n + g2.n - g1.l +1);
    vector<int> nlistsizes(g1.n + g2.n - g1.l +1);
    vector<int> morph1(g1.n);
    vector<int> morph2(g2.n);

    for (int i=0; i < g1.n; ++i) {
        morph1[i] = i;
    }
    for (int i=1; i < g2.l; ++i) {
        morph2[i] = g2.l-i;
    }
    morph2[0] = g1.n;
    for (int i=g2.l; i < g2.n; ++i) {
        morph2[i] = i+g1.n+1-g1.l;
    }

    for (int u = 0; u < g1.n; ++u) {
        nlistsizes[morph1[u]] = g1.list_sizes[u];
        for (int v : g1.al[u]) {
            nal[morph1[u]].push_back(morph1[v]);
        }
    }

    for (int u = 0; u < g2.n; ++u) {
        nlistsizes[morph2[u]] = g2.list_sizes[u];
        for (int v : g2.al[u]) {
            if (u < g2.l && v < g2.l && u > 0 && v > 0) continue;
            nal[morph2[u]].push_back(morph2[v]);
        }
    }

    for (int i=2; i+1 < g1.l; ++i) {
        nlistsizes[i] = 5;
    }

    return TwoPrecoloredPathsGraph(nal, nlistsizes);
}



TwoPrecoloredPathsGraph::TwoPrecoloredPathsGraph(const TwoPrecoloredPathsGraphCode& code) {

    n = 1;

    for (int i = 0; i < code.size(); ++i) {
        if (code[i] < 0) n++;
    }
    al = vector<vector<int>>(n);
    list_sizes = vector<int>(n);
    list_sizes[0] = 1;
    int cn = 0;
    vector<int> cv_stack;
    cv_stack.push_back(0);
    vector<int> zero_neigh;
    for (int i = 0; i < code.size(); ++i) {
        if (code[i] < 0) {
            al[cv_stack.back()].push_back(++cn);
            cv_stack.push_back(cn);
            list_sizes[cn] = code[i]-TwoPrecoloredPathsGraphCode::F_OFFSET;
        }
        else if (code[i] == 0) {
            int a = cv_stack.back();
            cv_stack.pop_back();
            int b = cv_stack.back();
            al[a].push_back(b);
        }
        else {
            al[cv_stack.back()].push_back(code[i]-1);
            if (code[i] == 1) {
                zero_neigh.push_back(cv_stack.back());
            }
        }
    }
    std::reverse(zero_neigh.begin(), zero_neigh.end());
    for (int x : zero_neigh) al[0].push_back(x);

    generate_ral_and_m();
    init_inherited_values();

}

void TwoPrecoloredPathsGraph::dfs_code(int u, int idx, int& c, vector<int>& an, TwoPrecoloredPathsGraphCode& code) const {
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

TwoPrecoloredPathsGraphCode TwoPrecoloredPathsGraph::compute_code_edge(int u, int v) const {
    TwoPrecoloredPathsGraphCode code = TwoPrecoloredPathsGraphCode();
    vector<int> assigned_numbers (n, -1);
    assigned_numbers[u] = 0;
    int c = 1;
    dfs_code(v, ral[v].at(u), c, assigned_numbers, code);
    return code;
}


TwoPrecoloredPathsGraphCode TwoPrecoloredPathsGraph::compute_code() const {
    TwoPrecoloredPathsGraphCode code;

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

bool TwoPrecoloredPathsGraph::test_no_l3_adjacent() const  {
    for (int u = 0; u < n; ++u) {
        if (list_sizes[u] == 3) {
            for (int v : al[u]) {
                if (list_sizes[v] == 3) return false;
            }
        }
    }
    return true;
}

bool TwoPrecoloredPathsGraph::test_criticality() const {
    if (n == 4) return true;
    if (!test_no_l3_adjacent()) return false;
    return !batch_test(compute_list_graph());
}