#include "PrecoloredPathAndVertexGraph.hh"
#include "ReducibilityTests.hh"
#include "debug_utility.hh"
#include <queue>
#include <functional>

using ll = long long;
using std::string;
using std::endl;

PrecoloredPathAndVertexGraphCode::PrecoloredPathAndVertexGraphCode() {
	
}
	
void PrecoloredPathAndVertexGraphCode::push_r(int x) {
    code.push_back(1+x);
}

void PrecoloredPathAndVertexGraphCode::push_f(int list_size) {
    code.push_back(F_OFFSET+list_size);
}

void PrecoloredPathAndVertexGraphCode::push_b() {
    code.push_back(0);
}

int PrecoloredPathAndVertexGraphCode::size() const {
    return code.size();
}

string PrecoloredPathAndVertexGraphCode::to_string() const { //can be made better
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

PrecoloredPathAndVertexGraphCode::PrecoloredPathAndVertexGraphCode(const string& s) {
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


int PrecoloredPathAndVertexGraphCode::operator[](int i) const {
    return code[i];
}


bool PrecoloredPathAndVertexGraphCode::operator<(const PrecoloredPathAndVertexGraphCode& b) const {
	for (int i=0; i < std::min((int)code.size(), b.size()); ++i) {
		if (code[i] < b[i]) return true;
		if (code[i] > b[i]) return false;
	}
	return (int)code.size() < b.size();
}


PrecoloredPathAndVertexGraph::PrecoloredPathAndVertexGraph() {

}

PrecoloredPathAndVertexGraph::PrecoloredPathAndVertexGraph(vector<vector<int>> _al, vector<int> _list_sizes) {
    n = _al.size();

    debug_assert(n >= 4);

    list_sizes = _list_sizes;
    m = 0;
    al = _al;
    generate_ral_and_m();
    init_inherited_values();
}

void PrecoloredPathAndVertexGraph::init_inherited_values() {
    precolored = vector<int>(n, 0);
    for (int i=0; i < n; ++i) {
        if (list_sizes[i] == 1) {
            precolored[i] = 1;
        }
    }
}

void PrecoloredPathAndVertexGraph::write(std::ostream& os) const {
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

void PrecoloredPathAndVertexGraph::write_prolog(std::ostream& os) const {
    os << "numVertices(" << n << ")." << endl;
    os << "numVerticesTSubgraph(" << 4 << ")." << endl;
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


PrecoloredPathAndVertexGraph PrecoloredPathAndVertexGraph::read(std::istream& is) {
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
	return PrecoloredPathAndVertexGraph(al, list_sizes);
}


PrecoloredPathAndVertexGraph PrecoloredPathAndVertexGraph::read_code(std::istream& is) {
    string s;
    is >> s;
    return PrecoloredPathAndVertexGraph(PrecoloredPathAndVertexGraphCode(s));
}


std::vector<PrecoloredPathAndVertexGraph> PrecoloredPathAndVertexGraph::fuse_paths(const PrecoloredPathAndVertexGraph& g1, const PrecoloredPathAndVertexGraph& g2) {
    vector<PrecoloredPathAndVertexGraph>  ans;
    for(int ul=3; ul <= 5; ++ul) {
        ans.push_back(PrecoloredPathAndVertexGraph::fuse_vertex(g1, g2, ul, true));
        ans.push_back(PrecoloredPathAndVertexGraph::fuse_vertex(g1, g2, ul, false));
    }
    return ans;

}

PrecoloredPathAndVertexGraph PrecoloredPathAndVertexGraph::fuse_vertex(const PrecoloredPathAndVertexGraph& g1, const PrecoloredPathAndVertexGraph& g2, int uls, bool reverse) {
    vector<int> morph1(g1.n);
    vector<int> morph2(g2.n);

    int u1 = g1.get_paths()[1][0];
    int u2 = g2.get_paths()[1][0];

    for (int i=0; i < g1.n; ++i) {
        morph1[i] = i;
    }
    for (int i=0; i < g2.n; ++i) {
        morph2[i] = i + g1.n - (i > u2);
    }
    morph2[u2] = morph1[u1];

    vector<vector<int> > nal (g1.n + g2.n - 1);
    vector<int> nls (g1.n + g2.n - 1);

    for (int u = 0; u < g2.n; ++u) {
        nls[morph2[u]] = g2.list_sizes[u];
        for (int v : g2.al[u]) {
            nal[morph2[u]].push_back(morph2[v]);
        }
    }

    if (reverse) {
        for (int u = 0; u < g2.n; ++u) {
            std::reverse(nal[morph2[u]].begin(), nal[morph2[u]].end());
        }
    }
    
    for (int u = 0; u < g1.n; ++u) {
        nls[morph1[u]] = g1.list_sizes[u];
        for (int v : g1.al[u]) {
            nal[morph1[u]].push_back(morph1[v]);
        }
    }



    nls[morph1[u1]] = uls;

    return PrecoloredPathAndVertexGraph(nal, nls);

}


//Returns path with orientation and vertex
//TODO: verify that orientation invariant is mantained, otherwise this could get chaotic
//TODO: does it work when the distance is 1?
vector<vector<int>> PrecoloredPathAndVertexGraph::get_paths() const {
    int pu, pv;
    pu = pv = -1;
    for (int u = 0; u < n && pu == -1; ++u) {
        if (precolored[u]) {
            for (int v : al[u]) {
                if (precolored[v]) {
                    pu = u;
                    pv = v;
                    break;
                }
            }
        }
    }
    int pu2 = -1;
    for (int u = 0; u < n; ++u) {
        if (u != pu && u != pv && precolored[u]) {
            pu2 = u;
        }
    }

    if (ral[pu].at(pv) != 0) std::swap(pu, pv);

    debug_assert(ral[pu].at(pv) == 0);

    return {{pu, pv}, {pu2}};
}


PrecoloredPathAndVertexGraph::PrecoloredPathAndVertexGraph(const PrecoloredPathAndVertexGraphCode& code) {

    n = 1;

    for (int i = 0; i < code.size(); ++i) {
        if (code[i] < 0) n++;
    }

    debug_assert(n >= 3);

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
            list_sizes[cn] = code[i]-PrecoloredPathAndVertexGraphCode::F_OFFSET;
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

void PrecoloredPathAndVertexGraph::dfs_code(int u, int idx, int& c, vector<int>& an, PrecoloredPathAndVertexGraphCode& code) const {
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

PrecoloredPathAndVertexGraphCode PrecoloredPathAndVertexGraph::compute_code_edge(int u, int v) const {
    PrecoloredPathAndVertexGraphCode code = PrecoloredPathAndVertexGraphCode();
    vector<int> assigned_numbers (n, -1);
    assigned_numbers[u] = 0;
    int c = 1;
    dfs_code(v, ral[v].at(u), c, assigned_numbers, code);
    return code;
}


PrecoloredPathAndVertexGraphCode PrecoloredPathAndVertexGraph::compute_code() const {
    PrecoloredPathAndVertexGraphCode code;

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


int PrecoloredPathAndVertexGraph::distance_between_paths() const {
    int pu, pv;
    vector<vector<int>> paths = get_paths();
    pu = paths[0][0];
    pv = paths[0][1];
    std::queue<int> q;
    vector<int> dist(n, -1);
    q.push(pu);
    q.push(pv);
    dist[pu] = dist[pv] = 0;
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

    int md = n+1;
    for (int u = 0; u < n; ++u) {
        if (u != pu && u != pv && precolored[u]) {
            md = std::min(md, dist[u]);
        }
    }
    return md;
}

PrecoloredPathAndVertexGraph PrecoloredPathAndVertexGraph::reverse() const {
    vector<vector<int>> nal(n);
    vector<int> nlistsizes(n);
    vector<int> morph(n);
    for (int i=0; i < n; ++i) {
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

    return PrecoloredPathAndVertexGraph(nal, nlistsizes);
}

bool PrecoloredPathAndVertexGraph::test_no_l3_adjacent() const  {
    for (int u = 0; u < n; ++u) {
        if (list_sizes[u] == 3) {
            for (int v : al[u]) {
                if (list_sizes[v] == 3) return false;
            }
        }
    }
    return true;
}

bool PrecoloredPathAndVertexGraph::test_criticality() const {
    if (n == 3) return true;
    //if (!test_no_l3_adjacent()) return false;
    return !batch_test(compute_list_graph());
}