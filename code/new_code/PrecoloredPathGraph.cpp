#include "PrecoloredPathGraph.hh"
#include "ReducibilityTests.hh"
#include "debug_utility.hh"

using ll = long long;
using std::string;
using std::endl;

PrecoloredPathGraphCode::PrecoloredPathGraphCode() {
	
}
	
void PrecoloredPathGraphCode::push_r(int x) {
    code.push_back(1+x);
}

void PrecoloredPathGraphCode::push_f(int list_size) {
    code.push_back(F_OFFSET+list_size);
}

void PrecoloredPathGraphCode::push_b() {
    code.push_back(0);
}

int PrecoloredPathGraphCode::size() const {
    return code.size();
}

string PrecoloredPathGraphCode::to_string() const { //can be made better
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

PrecoloredPathGraphCode::PrecoloredPathGraphCode(const string& s) {
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


int PrecoloredPathGraphCode::operator[](int i) const {
    return code[i];
}


bool PrecoloredPathGraphCode::operator<(const PrecoloredPathGraphCode& b) const {
	for (int i=0; i < std::min((int)code.size(), b.size()); ++i) {
		if (code[i] < b[i]) return true;
		if (code[i] > b[i]) return false;
	}
	return (int)code.size() < b.size();
}


PrecoloredPathGraph::PrecoloredPathGraph() {
    l = 0;
}

PrecoloredPathGraph::PrecoloredPathGraph(vector<vector<int>> _al, int _l, vector<int> _list_sizes) {
    l = _l;
    n = _al.size();
    list_sizes = _list_sizes;
    m = 0;
    al = _al;
    generate_ral_and_m();
    init_inherited_values();
}

void PrecoloredPathGraph::init_inherited_values() {
    precolored = vector<int>(n, 0);
    for (int i=0; i < l; ++i) {
        precolored[i] = 1;
        list_sizes[i] = 1;
    }
}

void PrecoloredPathGraph::write(std::ostream& os) const {
    os << n << " " << m << " " << l << endl;
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

void PrecoloredPathGraph::write_prolog(std::ostream& os) const {
    os << "numVertices(" << n << ")." << endl;
    os << "numVerticesTSubgraph(" << l << ")." << endl;
    for (int i=0; i < n; ++i) {
        os << "listSize(" << i << "," << list_sizes[i] << ")." << endl;
    }
    for (int i=0; i < n; ++i) {
        for (int j : al[i]) {
            if (i < j) {
                //TODO: does not take into account chords!
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


PrecoloredPathGraph PrecoloredPathGraph::read(std::istream& is) {
    int n, m, l;
	is >> n >> m >> l;
    vector<int> list_sizes(n);
    for (int i=0; i < n; ++i) is >> list_sizes[i];
	vector<vector<int> > al(n);
	for (int i=0; i < 2*m; ++i) {
		int u, v;
		is >> u >> v;
		al[u].push_back(v);
	}
	return PrecoloredPathGraph(al, l, list_sizes);
}


PrecoloredPathGraph PrecoloredPathGraph::read_code(std::istream& is) {
    string s;
    is >> s;
    return PrecoloredPathGraph(PrecoloredPathGraphCode(s));
}


PrecoloredPathGraph PrecoloredPathGraph::empty_cycle(int l, vector<int> ls) {
    int n = ls.size();
    vector<vector<int>> al(n);
    for (int i=0; i < n; ++i) {
        al[i].push_back((i+1)%n);
        al[i].push_back((i+n-1)%n);
    }
    return PrecoloredPathGraph(al, l, ls);
}

PrecoloredPathGraph PrecoloredPathGraph::empty_path(int l) {
    vector<vector<int>> al(l);
    al[0].push_back(1);
    for (int i=1; i < l-1; ++i) {
        al[i].push_back(i+1);
        al[i].push_back(i-1);
    }
    al[l-1].push_back(l-2);
    return PrecoloredPathGraph(al, l, vector<int>(l, 1));
}


PrecoloredPathGraph PrecoloredPathGraph::add_tripod(int s, int j, ll bm) const {

    // 1 <= j <= l-2
    int pj = j-1;
    int nj = j+1;
    
    vector<int> perm(n);
    for(int i=0; i <= pj; ++i) {
        perm[i] = i; 
    }
    for(int i=nj; i < l; ++i) {
        perm[i] = i+s-1;
    }
    perm[j] = l+s-1;
    for(int i=l; i < n; ++i) {
        perm[i] = i+s;
    }
    
    
    vector<vector<int>> nal (n + s);
    nal[perm[pj]].push_back(j);
    for (int u = 0; u < n; ++u) {
        for (int i = 0; i < (int)al[u].size(); ++i) {
            int v = al[u][i];
            nal[perm[u]].push_back(perm[v]);
            int nv = al[u][(i+1)%al[u].size()];
            if (u == j && (v == pj) && (nv == nj)) {
                for (ll z = j; z < j+s; ++z) {
                    nal[z].push_back((z+1));
                    if (bm&(1LL<<(z-j))) {
                        nal[perm[j]].push_back(z);
                        nal[z].push_back(perm[j]);
                    }
                    if (z == 0) nal[z].push_back(perm[pj]);
                    else nal[z].push_back(z-1);
                }
            }
            
        }
    }
    nal[perm[nj]].push_back(j+s-1);

    vector<int> nlistsizes (n + s);
    for (int u = 0; u < n; ++u) {
        nlistsizes[perm[u]] = list_sizes[u];
    }
    nlistsizes[perm[j]] = 5;
    for (int z=j; z < j+s; ++z) {
        nlistsizes[z] = 1;
    }
    
    return PrecoloredPathGraph(nal, l+s-1, nlistsizes);
}

PrecoloredPathGraph PrecoloredPathGraph::extend_path_chord(int s, int j) const {
    //0 <= j < l-1
    vector<vector<int>> nal(n+s);
    vector<int> perm(n);
    for (int i=0; i <= j; ++i) {
        perm[i] = i;
    }
    for (int i=j+1; i < n; ++i) {
        perm[i] = i+s;
    }

    nal[perm[j]].push_back(perm[j]+1);
    
    for (int i=j+1; i <= j+s; ++i) {
        nal[i].push_back(i+1);
        nal[i].push_back(i-1);
    }

    for (int u=0; u < n; ++u) {
        for (int v : al[u]) {
            nal[perm[u]].push_back(perm[v]);
        }
    } 

    nal[perm[j+1]].push_back(perm[j+1]-1);

    vector<int> nlistsizes(n+s);
    for (int u = 0; u < n; ++u) nlistsizes[perm[u]] = list_sizes[u];
    for (int i=j+1; i <= j+s; ++i) nlistsizes[i] = 1;

    return PrecoloredPathGraph(nal, l+s, nlistsizes);
}

PrecoloredPathGraph PrecoloredPathGraph::fuse_chord(const PrecoloredPathGraph& g1, const PrecoloredPathGraph& g2, int list_size_joint) {
    vector<vector<int>> nal(g1.n + g2.n - 2);
    vector<int> morph1(g1.n);
    vector<int> morph2(g2.n);
    for (int i=0; i < g1.l-1; ++i) {
        morph1[i] = i;
    }
    for (int i=1; i < g2.l; ++i) {
        morph2[i] = i+g1.l-3;
    }
    morph1[g1.l-1] = morph2[0] = g1.l+g2.l-3;
    for (int i=g1.l; i < g1.n; ++i) {
        morph1[i] = i + morph1[g1.l-1]-(g1.l-1);
    }
    for (int i=g2.l; i < g2.n; ++i) {
        morph2[i] = (i-g2.l) + morph1[g1.n-1] + 1;
    }

    for (int u = 1; u < g2.n; ++u) {
        for (int v : g2.al[u]) {
            nal[morph2[u]].push_back(morph2[v]);
        }
    }

    for (int u = 0; u < g1.n; ++u) {
        for (int v : g1.al[u]) {
            if (u == g1.l-2 && v == g1.l-1) continue;
            if (u == g1.l-1 && v == g1.l-2) continue;
            nal[morph1[u]].push_back(morph1[v]);
        }
    }

    // u=0
    {
        for (int v : g2.al[0]) {
            nal[morph2[0]].push_back(morph2[v]);
        }
    }

    vector<int> nlistsizes(g1.n + g2.n - 2);

    for (int u = 0; u < g1.n; ++u) {
        nlistsizes[morph1[u]] = g1.list_sizes[u];
    }
    for (int u = 0; u < g2.n; ++u) {
        nlistsizes[morph2[u]] = g2.list_sizes[u];
    }
    nlistsizes[morph2[0]] = list_size_joint;

    
    return PrecoloredPathGraph(nal, g1.l+g2.l-3, nlistsizes);

}

PrecoloredPathGraph PrecoloredPathGraph::fuse_articulation_point(const PrecoloredPathGraph& g1, const PrecoloredPathGraph& g2) {
    vector<vector<int>> nal(g1.n + g2.n - 1);
    vector<int> morph1(g1.n);
    vector<int> morph2(g2.n);
    for (int i=0; i < g1.l; ++i) {
        morph1[i] = i;
    }
    for (int i=0; i < g2.l; ++i) {
        morph2[i] = i+g1.l-1;
    }
    for (int i=g1.l; i < g1.n; ++i) {
        morph1[i] = (i-g1.l) + g1.l+g2.l-1;
    }
    for (int i=g2.l; i < g2.n; ++i) {
        morph2[i] = (i-g2.l) + g1.n + g2.l - 1;
    }

    for (int u = 0; u < g2.n; ++u) {
        for (int v : g2.al[u]) {
            nal[morph2[u]].push_back(morph2[v]);
        }
    }

    for (int u = 0; u < g1.n; ++u) {
        for (int v : g1.al[u]) {
            nal[morph1[u]].push_back(morph1[v]);
        }
    }

    

    vector<int> nlistsizes(g1.n + g2.n - 1);

    for (int u = 0; u < g1.n; ++u) {
        nlistsizes[morph1[u]] = g1.list_sizes[u];
    }
    for (int u = 0; u < g2.n; ++u) {
        nlistsizes[morph2[u]] = g2.list_sizes[u];
    }
    
    return PrecoloredPathGraph(nal, g1.l+g2.l-1, nlistsizes);

}

PrecoloredPathGraph PrecoloredPathGraph::reverse() const {
    vector<vector<int>> nal(n);
    vector<int> nlistsizes(n);
    vector<int> morph(n);

    for (int i=0; i < l; ++i) {
        morph[i] = l-1-i;
    }
    for (int i=l; i < n; ++i) {
        morph[i] = i;
    }

    for (int u = 0; u < n; ++u) {
        nlistsizes[morph[u]] = list_sizes[u];
        for (int v : al[u]) {
            nal[morph[u]].push_back(morph[v]);
        }
    }

    for (int i = 0; i < n; ++i) {
        std::reverse(nal[i].begin(), nal[i].end());
    }

    return PrecoloredPathGraph(nal, l, nlistsizes);
}

PrecoloredPathGraph::PrecoloredPathGraph(const PrecoloredPathGraphCode& code) {
    l = 1;
    n = 1;

    for (int i = 0; i < code.size(); ++i) {
        if (code[i] < 0) n++;
    }
    vector<vector<int>> nal = vector<vector<int>>(n);
    vector<int> nlistsizes = vector<int>(n);
    nlistsizes[0] = 1;
    int cn = 0;
    vector<int> cv_stack;
    cv_stack.push_back(0);
    vector<int> zero_neigh;
    vector<int> morph(n, -1);
    morph[0] = 0;
    for (int i = 0; i < code.size(); ++i) {
        if (code[i] < 0) {
            nal[cv_stack.back()].push_back(++cn);
            cv_stack.push_back(cn);
            nlistsizes[cn] = code[i]-PrecoloredPathGraphCode::F_OFFSET;
            if (nlistsizes[cn] == 1) {
                morph[cn] = l++;
            }
        }
        else if (code[i] == 0) {
            int a = cv_stack.back();
            cv_stack.pop_back();
            int b = cv_stack.back();
            nal[a].push_back(b);
        }
        else {
            nal[cv_stack.back()].push_back(code[i]-1);
            if (code[i] == 1) {
                zero_neigh.push_back(cv_stack.back());
            }
        }
    }
    std::reverse(zero_neigh.begin(), zero_neigh.end());
    for (int x : zero_neigh) nal[0].push_back(x);

    int cm = l;

    for (int i=0; i < n; ++i) {
        if (morph[i] == -1) {
            morph[i] = cm++;
        }
    }

    al = vector<vector<int>>(n);
    list_sizes = vector<int>(n);
    for (int u = 0; u < n; ++u) {
        list_sizes[morph[u]] = nlistsizes[u];
        for (int v : nal[u]) {
            al[morph[u]].push_back(morph[v]);
        }
    }

    generate_ral_and_m();
    init_inherited_values();

}

void PrecoloredPathGraph::dfs_code(int u, int idx, int& c, vector<int>& an, PrecoloredPathGraphCode& code) const {
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

PrecoloredPathGraphCode PrecoloredPathGraph::compute_code_edge(int u, int v) const {
    PrecoloredPathGraphCode code = PrecoloredPathGraphCode();
    vector<int> assigned_numbers (n, -1);
    assigned_numbers[u] = 0;
    int c = 1;
    dfs_code(v, ral[v].at(u), c, assigned_numbers, code);
    return code;
}


PrecoloredPathGraphCode PrecoloredPathGraph::compute_code() const {
    PrecoloredPathGraphCode code = std::min(compute_code_edge(0, 1), compute_code_edge(l-1, l-2));
    return code;
}

bool PrecoloredPathGraph::test_no_l3_adjacent() const  {
    for (int u = 0; u < n; ++u) {
        if (list_sizes[u] == 3) {
            for (int v : al[u]) {
                if (list_sizes[v] == 3) return false;
            }
        }
    }
    return true;
}

bool PrecoloredPathGraph::test_criticality() const {
    if (n == l) return true;
    if (!test_no_l3_adjacent()) return false;
    return !batch_test(compute_list_graph());
}