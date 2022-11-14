#include "Canvas.hh"
#include "ReducibilityTests.hh"

using ll = long long;
using std::string;
using std::endl;

CanvasCode::CanvasCode() {
	
}
	
void CanvasCode::push(int x) {
    code.push_back(x);
}

void CanvasCode::push_f() {
    code.push_back(-1);
}

void CanvasCode::push_b() {
    code.push_back(-2);
}

int CanvasCode::size() const {
    return code.size();
}

string CanvasCode::to_string() const { //can be made better
    string s = "";
    for (int i=0; i < size(); ++i) {
        if (code[i] == -1) s.push_back('F');
        else if (code[i] == -2) s.push_back('B');
        else if (code[i] <= 9) s.push_back('0'+code[i]);
        else s.push_back('a'+code[i]-10);
    }
    return s;
}

int CanvasCode::operator[](int i) const {
    return code[i];
}

CanvasCode::CanvasCode(const string& s) {
    for (int i=0; i < (int)s.length(); ++i) {
        if (s[i] == 'F') code.push_back(-1);
        else if (s[i] == 'B') code.push_back(-2);
        else if ('0' <= s[i] && s[i] <= '9') code.push_back(s[i]-'0');
        else code.push_back(s[i]-'a'+10);
    }
}

bool CanvasCode::operator<(const CanvasCode& b) const {
	for (int i=0; i < std::min((int)code.size(), b.size()); ++i) {
		if (code[i] < b[i]) return true;
		if (code[i] > b[i]) return false;
	}
	return (int)code.size() < b.size();
}

Canvas::Canvas() {
    l = 0;
}

Canvas::Canvas(vector<vector<int>> _al, int _l) {
    l = _l;
    n = _al.size();
    m = 0;
    al = _al;
    generate_ral_and_m();
    init_inherited_values();
}

void Canvas::init_inherited_values() {
    precolored = vector<int>(n, 0);
    list_sizes = vector<int>(n, 5);
    for (int i=0; i < l; ++i) {
        precolored[i] = 1;
        list_sizes[i] = 1;
    }
}

void Canvas::write(std::ostream& os) const {
    os << n << " " << m << " " << l << endl;
    for (int u = 0; u < n; ++u) {
        for (int v : al[u]) {
            os << u << " " << v << endl;
        }
    }
}

Canvas Canvas::read_code(std::istream& is) {
    string s;
    is >> s;
    return Canvas(CanvasCode(s));
}

Canvas Canvas::empty_cycle(int l) {
    vector<vector<int>> al(l);
    for (int i=0; i < l; ++i) {
        al[i].push_back((i+1)%l);
        al[i].push_back((i+l-1)%l);
    }
    return Canvas(al, l);
}

Canvas Canvas::fuse_chord(Canvas g1, Canvas g2, int j1, int j2, bool same_orientation) {
    vector<int> morph1(g1.n);
    vector<int> morph2(g2.n);

    morph1[j1] = 0;
    for (int i=1; i < g1.l; ++i) {
        morph1[(j1+i)%g1.l] = i+g2.l-2;
    }
    for (int i=g1.l; i < g1.n; ++i) {
        morph1[i] = i + g2.l-2;
    }

    morph2[j2] = 0;
    if (same_orientation) {
        for (int i=1; i < g2.l; ++i) {
            morph2[(j2+i)%g2.l] = i;
        }
    }
    else {
        for (int i=1; i < g2.l; ++i) {
            morph2[(j2-i+g2.l)%g2.l] = i;
        }
    }
    for (int i=g2.l; i < g2.n; ++i) {
        morph2[i] = i+g1.n-2;
    }

    vector<vector<int> > nal (g1.n + g2.n -2);
    vector<int> jal;
    for (int u = 0; u < g1.n; ++u) {
        for (int v : g1.al[u]) {
            if (u == (j1+1)%g1.l && v == j1) {
                continue;
            }
            if (u == j1) {
                jal.push_back(morph1[v]);
            }
            else {
                nal[morph1[u]].push_back(morph1[v]);
            }
        }
    }

    if (!same_orientation) {
        for (int u = 0; u < g2.n; ++u) {
            std::reverse(g2.al[u].begin(), g2.al[u].end());
        }
    }

    for (int u = 0; u < g2.n; ++u) {
        for (int v : g2.al[u]) {
            if (u == j2) {
                if (!same_orientation && v == (j2+1)%g2.l) continue;
                if (same_orientation && v == (j2-1+g2.l)%g2.l) continue;
            }
            
            nal[morph2[u]].push_back(morph2[v]);
            
        }
    }

    for (int x : jal) nal[0].push_back(x);

    Canvas res = Canvas(nal, g1.l+g2.l-2);

    return res;
}


Canvas Canvas::add_tripod(int s, int j, ll bm) const {
    int pj = (j-1+l)%l;
    int nj = (j+1+l)%l;
    
    vector<int> perm(n);
    for(int i=0; i < l; ++i) {
        perm[(nj+i)%l] = s+i; 
    }
    for(int i=l; i < n; ++i) {
        perm[i] = i+s;
    }
    
    
    vector<vector<int>> nal (n + s);
    nal[perm[pj]].push_back(0);
    for (int u = 0; u < n; ++u) {
        for (int i = 0; i < (int)al[u].size(); ++i) {
            int v = al[u][i];
            nal[perm[u]].push_back(perm[v]);
            int nv = al[u][(i+1)%al[u].size()];
            if (u == j && (v == pj) && (nv == nj)) {
                for (ll z = 0; z < s; ++z) {
                    nal[z].push_back((z+1));
                    if (bm&(1LL<<z)) {
                        nal[perm[j]].push_back(z);
                        nal[z].push_back(perm[j]);
                    }
                    if (z == 0) nal[z].push_back(perm[pj]);
                    else nal[z].push_back(z-1);
                }
            }
            
        }
    }
    nal[perm[nj]].push_back(s-1);
    
    return Canvas(nal, l+s-1);
}

Canvas::Canvas(const CanvasCode& code) {
    l = 1;
    n = 1;
    for (int i = 0; i < code.size(); ++i) {
        if (code[i] == -1) l++;
        else break;
    }

    for (int i = 0; i < code.size(); ++i) {
        if (code[i] == -1) n++;
    }
    al = vector<vector<int>>(n);
    int cn = 0;
    vector<int> cv_stack;
    cv_stack.push_back(0);
    vector<int> zero_neigh;
    for (int i = 0; i < code.size(); ++i) {
        if (code[i] == -1) {
            al[cv_stack.back()].push_back(++cn);
            cv_stack.push_back(cn);
        }
        else if (code[i] == -2) {
            int a = cv_stack.back();
            cv_stack.pop_back();
            int b = cv_stack.back();
            al[a].push_back(b);
        }
        else {
            al[cv_stack.back()].push_back(code[i]);
            if (code[i] == 0) {
                zero_neigh.push_back(cv_stack.back());
            }
        }
    }
    std::reverse(zero_neigh.begin(), zero_neigh.end());
    for (int x : zero_neigh) al[0].push_back(x);

    ral = vector< std::map<int, int> > (n);
    m = 0;
    for (int u = 0; u < n; ++u) {
        for (int i = 0; i < (int)al[u].size(); ++i) {
            int v = al[u][i];
            ral[u][v] = i; 
            m += 1;
        }
    }
    m /= 2;

    init_inherited_values();

}

void Canvas::dfs_code(int u, int idx, int& c, vector<int>& an, CanvasCode& code) const {
    if (an[u] != -1) {
        code.push(an[u]);
        return;
    }
    code.push_f();
    an[u] = c++;
    int als = (int)al[u].size();
    for (int i=1; i < als; ++i) {
        int v = al[u][(i+idx)%als];
        dfs_code(v, ral[v].at(u), c, an, code);
    }
    code.push_b();
}

CanvasCode Canvas::compute_code_edge(int u, int v) const {
    CanvasCode code = CanvasCode();
    vector<int> assigned_numbers (n, -1);
    assigned_numbers[u] = 0;
    int c = 1;
    dfs_code(v, ral[v].at(u), c, assigned_numbers, code);
    return code;
}

//Note: may give different codes for isomorphic graphs if the outer orientation is reversed 
CanvasCode Canvas::compute_code() const {
    CanvasCode code = std::min(compute_code_edge(0, 1), compute_code_edge(1, 0));
    for (int u = 1; u < l; ++u) {
        code = std::min(code, std::min(compute_code_edge(u, (u+1)%l), compute_code_edge((u+1)%l, u)));
    }
    return code;
}

bool Canvas::test_criticality() const {
    if (n == l) return true;
    return !recursive_reducibility_batch_test(compute_list_graph());
}