#include "PrecoloredPathAndTriangleGraph.hh"
#include "ReducibilityTests.hh"
#include "debug_utility.hh"
#include <queue>
#include <functional>
#include <set>

using ll = long long;
using std::string;
using std::endl;

PrecoloredPathAndTriangleGraphCode::PrecoloredPathAndTriangleGraphCode() {
	
}
	
void PrecoloredPathAndTriangleGraphCode::push_r(int x) {
    code.push_back(1+x);
}

void PrecoloredPathAndTriangleGraphCode::push_f(int list_size) {
    code.push_back(F_OFFSET+list_size);
}

void PrecoloredPathAndTriangleGraphCode::push_b() {
    code.push_back(0);
}

int PrecoloredPathAndTriangleGraphCode::size() const {
    return code.size();
}

string PrecoloredPathAndTriangleGraphCode::to_string() const { //can be made better
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

PrecoloredPathAndTriangleGraphCode::PrecoloredPathAndTriangleGraphCode(const string& s) {
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


int PrecoloredPathAndTriangleGraphCode::operator[](int i) const {
    return code[i];
}


bool PrecoloredPathAndTriangleGraphCode::operator<(const PrecoloredPathAndTriangleGraphCode& b) const {
	for (int i=0; i < std::min((int)code.size(), b.size()); ++i) {
		if (code[i] < b[i]) return true;
		if (code[i] > b[i]) return false;
	}
	return (int)code.size() < b.size();
}


PrecoloredPathAndTriangleGraph::PrecoloredPathAndTriangleGraph() {

}

PrecoloredPathAndTriangleGraph::PrecoloredPathAndTriangleGraph(vector<vector<int>> _al, vector<int> _list_sizes) {
    n = _al.size();

    debug_assert(n >= 5);

    list_sizes = _list_sizes;
    m = 0;
    al = _al;
    generate_ral_and_m();
    init_inherited_values();
    generate_path_and_triangle();
    
}

void PrecoloredPathAndTriangleGraph::generate_path_and_triangle() {
    path = vector<int>();
    triangle = vector<int>();
    for (int u=0; u < n; ++u) {
        if (precolored[u]) {
            int pc = 0;
            int pv = 0;
            for (int v : al[u]) {
                if (precolored[v]) {
                    pv = v;
                    pc++;
                }
            }
            if (pc == 1) {
                path = {u, pv};
                break;
            }
        }
    }

    //Path with outer face orientation
    debug_assert((int)path.size() == 2);
    if(al[path[0]][0] != path[1]) std::swap(path[0], path[1]);
    debug_assert(al[path[0]][0] == path[1]);

    for (int u=0; u < n; ++u) {
        if (precolored[u] && u != path[0] && u != path[1]) {
            triangle.push_back(u);
        }
    }
}

void PrecoloredPathAndTriangleGraph::init_inherited_values() {
    precolored = vector<int>(n, 0);
    for (int i=0; i < n; ++i) {
        if (list_sizes[i] == 1) {
            precolored[i] = 1;
        }
    }
}

void PrecoloredPathAndTriangleGraph::write(std::ostream& os) const {
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



PrecoloredPathAndTriangleGraph PrecoloredPathAndTriangleGraph::read(std::istream& is) {
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
	return PrecoloredPathAndTriangleGraph(al, list_sizes);
}


PrecoloredPathAndTriangleGraph PrecoloredPathAndTriangleGraph::read_code(std::istream& is) {
    string s;
    is >> s;
    return PrecoloredPathAndTriangleGraph(PrecoloredPathAndTriangleGraphCode(s));
}



vector<PrecoloredPathAndTriangleGraph> PrecoloredPathAndTriangleGraph::fuse_canvas_and_path_triangle_in_path(const Canvas& c, const PrecoloredPathGraph& p) {
    debug_assert(p.l == 7);
    vector<PrecoloredPathAndTriangleGraph> ans;
    for (int i=0; i < c.l; ++i) {
        for (int dj=std::max(2, c.l-2-6); dj <= std::min(4, c.l-6); ++dj) {
            PrecoloredPathAndTriangleGraph ppt = fuse_canvas_and_path_triangle_in_path_indices(c, p, i, (i+dj)%c.l);
            ppt.list_sizes[(i+dj+1)%c.l] = 5;
            ppt.list_sizes[(i+dj+4)%c.l] = 5;
            vector<int> variable_list_size_vertices;
            for (int k=2; k <= dj; ++k) {
                variable_list_size_vertices.push_back((i+k)%c.l);
            }
            for (int k=(i+dj+5)%c.l; k != i; k=(k+1)%c.l) {
                variable_list_size_vertices.push_back(k);
            }
            int pow3 = 1;
            int vlsvs = variable_list_size_vertices.size();
            for (int k=0; k < vlsvs; ++k) pow3 *= 3;
            for (int mask=0; mask < pow3; ++mask) {
                int x = mask;
                for (int k=0; k < vlsvs; ++k) {
                    ppt.list_sizes[variable_list_size_vertices[k]] = (x%3)+3;
                    x /= 3;
                }
                PrecoloredPathAndTriangleGraph nppt = PrecoloredPathAndTriangleGraph(ppt.al, ppt.list_sizes);
                if(nppt.test_no_l3_adjacent()) ans.push_back(nppt);
            }
        }
    }
    return ans;
}

vector<PrecoloredPathAndTriangleGraph> PrecoloredPathAndTriangleGraph::fuse_canvas_and_path_triangle_in_canvas(const Canvas& c, const PrecoloredPathGraph& p) {
    debug_assert(p.l == 6);
    vector<PrecoloredPathAndTriangleGraph> ans;
    for (int i=0; i < c.l; ++i) {
        for (int dj=std::max(2, c.l-2-7); dj <= std::min(4, c.l-7); ++dj) {
            PrecoloredPathAndTriangleGraph ppt = fuse_canvas_and_path_triangle_in_canvas_indices(c, p, i, (i+dj)%c.l);
            ppt.list_sizes[(i+dj+1)%c.l] = 5;
            ppt.list_sizes[(i+dj+5)%c.l] = 5;
            vector<int> variable_list_size_vertices;
            for (int k=2; k <= dj; ++k) {
                variable_list_size_vertices.push_back((i+k)%c.l);
            }
            for (int k=(i+dj+6)%c.l; k != i; k=(k+1)%c.l) {
                variable_list_size_vertices.push_back(k);
            }
            int pow3 = 1;
            int vlsvs = variable_list_size_vertices.size();
            for (int k=0; k < vlsvs; ++k) pow3 *= 3;
            for (int mask=0; mask < pow3; ++mask) {
                int x = mask;
                for (int k=0; k < vlsvs; ++k) {
                    ppt.list_sizes[variable_list_size_vertices[k]] = (x%3)+3;
                    x /= 3;
                }

                PrecoloredPathAndTriangleGraph nppt = PrecoloredPathAndTriangleGraph(ppt.al, ppt.list_sizes);
                if(nppt.test_no_l3_adjacent()) ans.push_back(nppt);
            }
        }
    }
    return ans;
}

vector<CanvasWithIndices> PrecoloredPathAndTriangleGraph::critical_canvases_with_indices_triangle_in_path(Canvas c) {
    vector<CanvasWithIndices> ans;
    std::set<CanvasCode> edge_codes;
    for (int i=0; i < c.l; ++i) {
        if(edge_codes.find(c.compute_code_edge(i, (i+1)%c.l)) != edge_codes.end()) {
            continue;
        }
        edge_codes.insert(c.compute_code_edge(i, (i+1)%c.l));
        for (int dj=std::max(2, c.l-2-6); dj <= std::min(4, c.l-6); ++dj) {
            ListGraph g = ListGraph(c.al, c.list_sizes);
            //g.list_sizes[(i+dj+1)%c.l] = 5;
            //g.list_sizes[(i+dj+4)%c.l] = 5;
            vector<int> variable_list_size_vertices;
            for (int k=2; k < dj; ++k) {
                variable_list_size_vertices.push_back((i+k)%c.l);
            }
            for (int k=(i+dj+6)%c.l; k != i; k=(k+1)%c.l) {
                variable_list_size_vertices.push_back(k);
            }
            int pow3 = 1;
            int vlsvs = variable_list_size_vertices.size();
            for (int k=0; k < vlsvs; ++k) pow3 *= 3;
            for (int mask=0; mask < pow3; ++mask) {
                int x = mask;
                for (int k=0; k < vlsvs; ++k) {
                    g.list_sizes[variable_list_size_vertices[k]] = (x%3)+3;
                    x /= 3;
                }
                if (!batch_reducible_test(g)) {
                    CanvasWithIndices cwi;
                    //cwi.c = c;
                    cwi.i = i;
                    cwi.dj = dj;
                    cwi.mask = mask;
                    ans.push_back(cwi);
                }
            }
        }
    }
    return ans;
}

vector<CanvasWithIndices> PrecoloredPathAndTriangleGraph::critical_canvases_with_indices_triangle_in_canvas(Canvas c) {
    vector<CanvasWithIndices> ans;
    std::set<CanvasCode> edge_codes;
    for (int i=0; i < c.l; ++i) {
        if(edge_codes.find(c.compute_code_edge(i, (i+1)%c.l)) != edge_codes.end()) {
            continue;
        }
        edge_codes.insert(c.compute_code_edge(i, (i+1)%c.l));
        for (int dj=std::max(2, c.l-2-7); dj <= std::min(4, c.l-7); ++dj) {
            ListGraph g = ListGraph(c.al, c.list_sizes);
            //g.list_sizes[(i+dj+1)%c.l] = 5;
            //g.list_sizes[(i+dj+5)%c.l] = 5;
            vector<int> variable_list_size_vertices;
            for (int k=2; k < dj; ++k) {
                variable_list_size_vertices.push_back((i+k)%c.l);
            }
            for (int k=(i+dj+7)%c.l; k != i; k=(k+1)%c.l) {
                variable_list_size_vertices.push_back(k);
            }
            int pow3 = 1;
            int vlsvs = variable_list_size_vertices.size();
            for (int k=0; k < vlsvs; ++k) pow3 *= 3;
            for (int mask=0; mask < pow3; ++mask) {
                int x = mask;
                for (int k=0; k < vlsvs; ++k) {
                    g.list_sizes[variable_list_size_vertices[k]] = (x%3)+3;
                    x /= 3;
                }
                if (!batch_reducible_test(g)) {
                    CanvasWithIndices cwi;
                    //cwi.c = c;
                    cwi.i = i;
                    cwi.dj = dj;
                    cwi.mask = mask;
                    ans.push_back(cwi);
                }
               
            }
        }
    }
    return ans;
}

vector<PrecoloredPathAndTriangleGraph> PrecoloredPathAndTriangleGraph::fuse_canvas_with_indices_and_path_triangle_in_path(const Canvas& c,const CanvasWithIndices& cwi, const PrecoloredPathGraph& p) {
    debug_assert(p.l == 7);
    int i = cwi.i;
    int dj = cwi.dj;
    int mask = cwi.mask;
    PrecoloredPathAndTriangleGraph ppt = PrecoloredPathAndTriangleGraph::fuse_canvas_and_path_triangle_in_path_indices(c, p, i, (i+dj)%c.l);

    vector<PrecoloredPathAndTriangleGraph> ans;
    vector<int> variable_list_size_vertices;
    for (int k=2; k < dj; ++k) {
        variable_list_size_vertices.push_back((i+k)%c.l);
    }
    for (int k=(i+dj+6)%c.l; k != i; k=(k+1)%c.l) {
        variable_list_size_vertices.push_back(k);
    }
    int pow3 = 1;
    int vlsvs = variable_list_size_vertices.size();
    for (int k=0; k < vlsvs; ++k) pow3 *= 3;
    
    int x = mask;
    for (int k=0; k < vlsvs; ++k) {
        ppt.list_sizes[variable_list_size_vertices[k]] = (x%3)+3;
        x /= 3;
    }

    ppt.list_sizes[(i+dj+1)%c.l] = 5;
    ppt.list_sizes[(i+dj+4)%c.l] = 5;
    
    vector<int> variable_list_size_vertices2 = {(i+dj)%c.l, (i+dj+5)%c.l};

    for (int mask2=0; mask2 < 9; ++mask2) {
        int x = mask2;
        for (int k=0; k < 2; ++k) {
            ppt.list_sizes[variable_list_size_vertices2[k]] = (x%3)+3;
            x /= 3;
        }
        PrecoloredPathAndTriangleGraph nppt = PrecoloredPathAndTriangleGraph(ppt.al, ppt.list_sizes);
        if(nppt.test_no_l3_adjacent()) ans.push_back(nppt);
    }

    return ans;
        
}

vector<PrecoloredPathAndTriangleGraph> PrecoloredPathAndTriangleGraph::fuse_canvas_with_indices_and_path_triangle_in_canvas(const Canvas& c, const CanvasWithIndices& cwi, const PrecoloredPathGraph& p) {
    debug_assert(p.l == 6);
    int i = cwi.i;
    int dj = cwi.dj;
    int mask = cwi.mask;
    PrecoloredPathAndTriangleGraph ppt = PrecoloredPathAndTriangleGraph::fuse_canvas_and_path_triangle_in_canvas_indices(c, p, i, (i+dj)%c.l);

    vector<PrecoloredPathAndTriangleGraph> ans;
    vector<int> variable_list_size_vertices;
    for (int k=2; k < dj; ++k) {
        variable_list_size_vertices.push_back((i+k)%c.l);
    }
    for (int k=(i+dj+7)%c.l; k != i; k=(k+1)%c.l) {
        variable_list_size_vertices.push_back(k);
    }
    int pow3 = 1;
    int vlsvs = variable_list_size_vertices.size();
    for (int k=0; k < vlsvs; ++k) pow3 *= 3;
    
    int x = mask;
    for (int k=0; k < vlsvs; ++k) {
        ppt.list_sizes[variable_list_size_vertices[k]] = (x%3)+3;
        x /= 3;
    }

    ppt.list_sizes[(i+dj+1)%c.l] = 5;
    ppt.list_sizes[(i+dj+5)%c.l] = 5;
    
    vector<int> variable_list_size_vertices2 = {(i+dj)%c.l, (i+dj+6)%c.l};

    for (int mask2=0; mask2 < 9; ++mask2) {
        int x = mask2;
        for (int k=0; k < 2; ++k) {
            ppt.list_sizes[variable_list_size_vertices2[k]] = (x%3)+3;
            x /= 3;
        }
        PrecoloredPathAndTriangleGraph nppt = PrecoloredPathAndTriangleGraph(ppt.al, ppt.list_sizes);
        if(nppt.test_no_l3_adjacent()) ans.push_back(nppt);
    }

    return ans;
        
}

PrecoloredPathAndTriangleGraph PrecoloredPathAndTriangleGraph::fuse_canvas_and_path_triangle_in_path_indices(const Canvas& c, const PrecoloredPathGraph& p, int i, int j) {
    int n = c.n + p.n - 6;
    vector<vector<int>> al(n);
    vector<int> list_sizes(n);
    vector<int> morph_c(c.n, -1);
    vector<int> morph_p(p.n, -1);
    for (int i=0; i < c.n; ++i) {
        morph_c[i] = i;
    }
    morph_p[0] = j;
    morph_p[1] = (j+1)%c.l;
    morph_p[2] = (j+2)%c.l;
    morph_p[3] = c.n;
    morph_p[4] = (j+3)%c.l;
    morph_p[5] = (j+4)%c.l;
    morph_p[6] = (j+5)%c.l;
    for (int i=7; i < p.n; ++i) {
        morph_p[i] = c.n + i - 6;
    }

    vector<int> repeated_vertices_p = {0,1,2,4,5,6};
    for (int u=0; u < p.n; ++u) {
        list_sizes[morph_p[u]] = p.list_sizes[u];
        for (int v : p.al[u]) {
            if (std::count(repeated_vertices_p.begin(), repeated_vertices_p.end(), u) && std::count(repeated_vertices_p.begin(), repeated_vertices_p.end(), v)) continue;
            al[morph_p[u]].push_back(morph_p[v]);
        }
        std::reverse(al[morph_p[u]].begin(), al[morph_p[u]].end());
    }

    vector<int> last_vertex_al;

    for (int u=0; u < c.n; ++u) {
        list_sizes[morph_c[u]] = c.list_sizes[u];
        for (int v : c.al[u]) {
            if (u == (j+5)%c.l) {
                last_vertex_al.push_back(morph_c[v]);
            }
            else {
                al[morph_c[u]].push_back(morph_c[v]);
            }
        }
    }
    std::reverse(al[(j+5)%c.l].begin(), al[(j+5)%c.l].end());
    std::reverse(last_vertex_al.begin(), last_vertex_al.end());
    for (int x : last_vertex_al) al[(j+5)%c.l].push_back(x);
    std::reverse(al[(j+5)%c.l].begin(), al[(j+5)%c.l].end());

    for (int k=(i+2)%c.l; k != (j+2)%c.l; k=(k+1)%c.l) {
        list_sizes[morph_c[k]] = 3;
    }
    for (int k=(j+4)%c.l; k != i; k=(k+1)%c.l) {
        list_sizes[morph_c[k]] = 3;
    }

    return PrecoloredPathAndTriangleGraph(al, list_sizes);
 }


PrecoloredPathAndTriangleGraph PrecoloredPathAndTriangleGraph::fuse_canvas_and_path_triangle_in_canvas_indices(const Canvas& c, const PrecoloredPathGraph& p, int i, int j) {
    int n = c.n + p.n - 6;
    vector<vector<int>> al(n);
    vector<int> list_sizes(n);
    vector<int> morph_c(c.n, -1);
    vector<int> morph_p(p.n, -1);
    for (int i=0; i < c.n; ++i) {
        morph_c[i] = i;
    }
    morph_p[0] = j;
    morph_p[1] = (j+1)%c.l;
    morph_p[2] = (j+2)%c.l;
    morph_p[3] = (j+4)%c.l;
    morph_p[4] = (j+5)%c.l;
    morph_p[5] = (j+6)%c.l;
    for (int i=6; i < p.n; ++i) {
        morph_p[i] = c.n + i - 6;
    }

    vector<int> repeated_vertices_p = {0,1,2,3,4,5};
    for (int u=0; u < p.n; ++u) {
        list_sizes[morph_p[u]] = p.list_sizes[u];
        for (int v : p.al[u]) {
            if (std::count(repeated_vertices_p.begin(), repeated_vertices_p.end(), u) && std::count(repeated_vertices_p.begin(), repeated_vertices_p.end(), v)) continue;
            al[morph_p[u]].push_back(morph_p[v]);
        }
        std::reverse(al[morph_p[u]].begin(), al[morph_p[u]].end());
    }

    al[morph_p[2]].push_back(morph_p[3]);
    al[morph_p[3]].push_back(morph_p[2]);

    vector<int> last_vertex_al;

    for (int u=0; u < c.n; ++u) {
        list_sizes[morph_c[u]] = c.list_sizes[u];
        for (int v : c.al[u]) {
            if (u == (j+6)%c.l) {
                last_vertex_al.push_back(morph_c[v]);
            }
            else {
                al[morph_c[u]].push_back(morph_c[v]);
            }
        }
    }
    std::reverse(al[(j+6)%c.l].begin(), al[(j+6)%c.l].end());
    std::reverse(last_vertex_al.begin(), last_vertex_al.end());
    for (int x : last_vertex_al) al[(j+6)%c.l].push_back(x);
    std::reverse(al[(j+6)%c.l].begin(), al[(j+6)%c.l].end());

    for (int k=(i+2)%c.l; k != (j+2)%c.l; k=(k+1)%c.l) {
        list_sizes[morph_c[k]] = 3;
    }
    for (int k=(j+5)%c.l; k != i; k=(k+1)%c.l) {
        list_sizes[morph_c[k]] = 3;
    }

    return PrecoloredPathAndTriangleGraph(al, list_sizes);
 }








PrecoloredPathAndTriangleGraph::PrecoloredPathAndTriangleGraph(const PrecoloredPathAndTriangleGraphCode& code) {

    n = 1;

    for (int i = 0; i < code.size(); ++i) {
        if (code[i] < 0) n++;
    }

    debug_assert(n >= 5);

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
            list_sizes[cn] = code[i]-PrecoloredPathAndTriangleGraphCode::F_OFFSET;
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
    generate_path_and_triangle();

}

void generate_path_and_triangle() {

}

void PrecoloredPathAndTriangleGraph::dfs_code(int u, int idx, int& c, vector<int>& an, PrecoloredPathAndTriangleGraphCode& code) const {
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

PrecoloredPathAndTriangleGraphCode PrecoloredPathAndTriangleGraph::compute_code_edge(int u, int v) const {
    PrecoloredPathAndTriangleGraphCode code = PrecoloredPathAndTriangleGraphCode();
    vector<int> assigned_numbers (n, -1);
    assigned_numbers[u] = 0;
    int c = 1;
    dfs_code(v, ral[v].at(u), c, assigned_numbers, code);
    return code;
}


PrecoloredPathAndTriangleGraphCode PrecoloredPathAndTriangleGraph::compute_code() const {
    
    return compute_code_edge(path[0], path[1]);
}


int PrecoloredPathAndTriangleGraph::distance_between_path_and_triangle() const {
    
    std::queue<int> q;
    vector<int> dist(n, -1);
    q.push(path[0]);
    q.push(path[1]);
    dist[path[0]] = dist[path[1]] = 0;
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
    return std::min(dist[triangle[0]], std::min(dist[triangle[1]], dist[triangle[2]]));
}



bool PrecoloredPathAndTriangleGraph::test_no_l3_adjacent() const  {
    for (int u = 0; u < n; ++u) {
        if (list_sizes[u] == 3) {
            for (int v : al[u]) {
                if (list_sizes[v] == 3) return false;
            }
        }
    }
    return true;
}

bool PrecoloredPathAndTriangleGraph::test_no_l4_adjacent_to_triangle() const  {
    for (int i = 0; i < 3; ++i) {
        int u = triangle[i];
        for (int v : al[u]) {
            if (list_sizes[v] <= 4 && std::count(triangle.begin(), triangle.end(), v) == 0) return false;
        }
        
    }
    return true;
}


int PrecoloredPathAndTriangleGraph::count_l3_adjacent_pairs() const  {
    int cnt = 0;
    for (int u = 0; u < n; ++u) {
        if (list_sizes[u] == 3) {
            for (int v : al[u]) {
                if (v > u && list_sizes[v] == 3) cnt++;
            }
        }
    }
    return cnt;
}

bool PrecoloredPathAndTriangleGraph::test_criticality() const {
    if (n == 5) return true;
    //if (!test_no_l3_adjacent()) return false;
    return !batch_test(compute_list_graph());
}

