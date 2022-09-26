#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<map>
#include<unordered_map>
#include<utility>

#ifdef DEBUG
const bool DEBUG_MODE = true;
#else
const bool DEBUG_MODE = false;
#endif


using std::vector;
using std::string;
using std::pair;
using std::unordered_map;

using std::cerr;
using std::endl;

using ll = long long;

struct Code {
	vector<int> code;
	
	Code() {
	
	}
	
	void push(int x) {
		code.push_back(x);
	}
	
	void push_f() {
		code.push_back(-1);
	}
	
	void push_b() {
		code.push_back(-2);
	}
	
	int size() const {
		return code.size();
	}
	
	string to_string() const { //can be made better
		string s = "";
		for (int i=0; i < size(); ++i) {
			if (code[i] == -1) s.push_back('F');
			else if (code[i] == -2) s.push_back('B');
			else if (code[i] <= 9) s.push_back('0'+code[i]);
			else s.push_back('a'+code[i]-10);
		}
		return s;
	}
	int operator[](int i) const {
		return code[i];
	}

	
};


bool operator<(const Code& a, const Code& b) {
	for (int i=0; i < std::min(a.size(), b.size()); ++i) {
		if (a[i] < b[i]) return true;
		if (a[i] > b[i]) return false;
	}
	return a.size() < b.size();
}



struct DFSGraph {
	int n;
	
	vector<vector<int> > al;

	vector<vector<int> > dfs_children;
	vector<int> dfs_parent;
	vector<int> dfs_num;
	vector<int> dfs_low;
	int dfs_count;

	bool is_root_ac = false;
	
	void dt_dfs(int v, int par) {
		dfs_parent[v] = par;
		dfs_num[v] = dfs_low[v] = dfs_count++;
		for(int u : al[v]) {
			if(u == par) {
				//Nothing
			}
			else if(dfs_num[u] == -1) {
				dfs_children[v].push_back(u);
				dt_dfs(u, v);
				dfs_low[v] = std::min(dfs_low[v], dfs_low[u]);
			}
			else {
				dfs_low[v] = std::min(dfs_low[v], dfs_num[u]);
			}
		}
	}

	void dt_dfs_cedges(vector<vector<pair<int, int>>>& cedges, vector<pair<int, int>>& edge_stack, int v, int par) {
		dfs_parent[v] = par;
		dfs_num[v] = dfs_low[v] = dfs_count++;
		for(int u : al[v]) {
			if(u == par) {
				//Nothing
			}
			else if(dfs_num[u] == -1) {
				dfs_children[v].push_back(u);
				edge_stack.emplace_back(v, u);
				dt_dfs_cedges(cedges, edge_stack, u, v);
				dfs_low[v] = std::min(dfs_low[v], dfs_low[u]);

				if((par == -1 && is_root_ac) || (par != -1 && dfs_low[u] >= dfs_num[v])) {
					vector<pair<int, int>> comp;
					while(edge_stack.back().first != v || edge_stack.back().second != u) {
						comp.push_back(edge_stack.back());
						edge_stack.pop_back();
					}
					comp.push_back(edge_stack.back());
					edge_stack.pop_back();
					cedges.push_back(comp);
				}
			}
			else {
				dfs_low[v] = std::min(dfs_low[v], dfs_num[u]);
				if(dfs_num[u] < dfs_num[v]) {
					edge_stack.emplace_back(v, u);
				}
			}
		}
	}

	void generate_dfs_tree(int root) {
		dfs_children = vector<vector<int>>(n, vector<int>());
		dfs_parent = vector<int>(n);
		dfs_count = 0;
		dfs_num = vector<int>(n, -1);
		dfs_low = vector<int>(n, -1);
		dt_dfs(root, -1);
	}

	void generate_edge_partition(vector<vector<pair<int, int>>>& cedges, vector<pair<int, int>>& edge_stack, int root) {
		generate_dfs_tree(root);
		is_root_ac = (dfs_children[root].size() > 1);

		dfs_children = vector<vector<int>>(n, vector<int>());
		dfs_parent = vector<int>(n);
		dfs_count = 0;
		dfs_num = vector<int>(n, -1);
		dfs_low = vector<int>(n, -1);
		dt_dfs_cedges(cedges, edge_stack, root, -1);
		if(!edge_stack.empty()) cedges.push_back(edge_stack);
	}
	
	vector<DFSGraph> partition_biconnected() {
		if (n == 0) return vector<DFSGraph>();
		vector<vector<pair<int, int>>> cedges;
		vector<pair<int, int>> edge_stack;
		generate_edge_partition(cedges, edge_stack, 0);
		vector<DFSGraph> comp;
		for(vector<pair<int,int>>& vec : cedges) {
			DFSGraph h;
			h.n = 0;
			//h.m = vec.size();
			unordered_map<int, int> rmorph;
			for(pair<int, int> e : vec) {
				if(rmorph.find(e.first) == rmorph.end()) {
					rmorph[e.first] = h.n++;
				}
				if(rmorph.find(e.second) == rmorph.end()) {
					rmorph[e.second] = h.n++;
				}
			}
			h.al = vector<vector<int> >(h.n);
			for(pair<int, int> e : vec) {
				int u = rmorph[e.first];
				int v = rmorph[e.second];
				h.al[u].push_back(v);
				h.al[v].push_back(u);
			}
			comp.push_back(h);
		}
		return comp;
	}
	
	bool is_clique() {
		for (int u = 0; u < n; ++u) {
			if ((int)al[u].size() < n-1) return false;
		}
		return true;
	}
	
	bool is_connected() {
		generate_dfs_tree(0);
		return dfs_count == n;
	}
	
	bool is_odd_cycle() {
		if (n % 2 == 0) return false;
		for (int u = 0; u < n; ++u) {
			if ((int)al[u].size() != 2) return false;
		}
		if (!is_connected()) return false;
		return true;
	}
};

struct PlaneGraph {
	int n; //num vertices
	int m; //num edges
	int l; //outer cycle length
	//vertices 0, 1, ..., l-1 are in the outer cycle in ccw order
	//in vertices in the outer cycle, vertex +1 should be the first one and vertex -1 the last one
	
	vector< vector <int> > al; //adjacency list representation, incl orientation
	vector< std::map<int, int> > ral;
	vector< vector <int> > am; //adjacency matrix representation
	
	
	PlaneGraph(int _l, vector< vector <int> > _al) {
		l = _l;
		al = _al;
		n = al.size();
		am = vector< vector<int> >(n, vector<int>(n, 0));
		ral = vector< std::map<int, int> > (n);
		m = 0;
		for (int u = 0; u < n; ++u) {
			for (int i = 0; i < (int)al[u].size(); ++i) {
				int v = al[u][i];
				ral[u][v] = i; 
				am[u][v] = 1;
				m += 1;
			}
		}
		m /= 2;
	}
	
	void dfs_code(int u, int idx, int& c, vector<int>& an, Code& code) {
		if (an[u] != -1) {
			code.push(an[u]);
			return;
		}
		code.push_f();
		an[u] = c++;
		int als = (int)al[u].size();
		for (int i=1; i < als; ++i) {
			int v = al[u][(i+idx)%als];
			dfs_code(v, ral[v][u], c, an, code);
		}
		code.push_b();
	}
	
	Code compute_code_edge(int u, int v) {
		Code code = Code();
		vector<int> assigned_numbers (n, -1);
		assigned_numbers[u] = 0;
		int c = 1;
		dfs_code(v, ral[v][u], c, assigned_numbers, code);
		return code;
	}
	
	Code compute_code() {
		Code code = std::min(compute_code_edge(0, 1), compute_code_edge(1, 0));
		for (int u = 1; u < l; ++u) {
			code = std::min(code, std::min(compute_code_edge(u, (u+1)%l), compute_code_edge((u+1)%l, u)));
		}
		return code;
	}
	
	PlaneGraph add_tripod(int s, int j, ll bm) {
	
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
		
		return PlaneGraph(l+s-1, nal);
	}

	static PlaneGraph fuse_chord(PlaneGraph g1, PlaneGraph g2, int j1, int j2, bool same_orientation) {
		//TODO: fix bugs
		if (DEBUG_MODE) {
			cerr << "fuse_chord(" << g1.compute_code().to_string() << ", " << g2.compute_code().to_string() << ", " 
			<< j1 << ", " << j2 << ", " << same_orientation << ")" << endl;
			//cerr << g1.n << " " << g2.n << endl;
			//cerr << g1.l << " " << g2.l << endl;
		}

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

		for (int u = 0; u < g1.n; ++u) {
			for (int v : g1.al[u]) {

				if (DEBUG_MODE) {
					//cerr << morph1[u] << endl;
				}

				if (u == (j1+1)%g1.l && v == j1) {
					continue;
				}

				nal[morph1[u]].push_back(morph1[v]);
			}
		}

		for (int u = 0; u < g2.n; ++u) {
			for (int v : g2.al[u]) {
				if (u == j2) {
					if (same_orientation && v == (j2+1)%g2.l) continue;
					if (!same_orientation && v == (j2-1+g2.l)%g2.l) continue;
				}
				nal[morph2[u]].push_back(morph2[v]);
			}
		}

		return PlaneGraph(g1.l+g2.l-2, nal);
	}
	
	
	bool degree_test() {
		for (int u = l; u < n; ++u) {
			if (al[u].size() < 5) return false;
		}
		return true;
	}
	
	int deficiency() {
		return m-l-3*(n-l);
	}
	
	bool deficiency_test() {
		return (m == l || deficiency() >= 1);
	}
	
	//v and w are adjacent in the adjacency list of u
	bool al_adjacent(int u, int v, int w) {
		int vi = ral[u][v];
		int wi = ral[u][w];
		int als = al[u].size();
		return (vi == wi+1 || wi == vi+1 || (vi == 0 && wi == als-1) || (wi == 0 && vi == als-1));
	}
	
	bool check_gadget4(int u, int v, int w, int x) {
		if(am[u][v] && am[u][w] && am[u][x] && am[v][w] && am[w][x]) {
			if(al_adjacent(u, v, w) && al_adjacent(u, w, x)) {
				if(al[u].size() <= 5 && al[v].size() <= 5 && al[w].size() <= 6 && al[x].size() <= 5) {
					return true;
				}
			}
		}
		return false;
	}
	
	bool gadget4_test() {
		for (int u = l; u < n; ++u) {
			for (int v = l; v < n; ++v) {
				for (int w = l; w < n; ++w) {
					for (int x = l; x < n; ++x) {
						if(check_gadget4(u, v, w, x)) return false;
					}
				}
			}
		}
		return true;
	}
	
	bool biconnected_deg5_components_test() {

		if (DEBUG_MODE) {
			cerr << "HEY" << endl;
		}

		vector<int> morph(n, -1);
		DFSGraph deg5graph;
		deg5graph.n = 0;
		for (int u = l; u < n; ++u) {
			int deg = al[u].size();
			if (deg == 5) {
				morph[u] = deg5graph.n++;
			}
		}

		if(DEBUG_MODE) {
			cerr << "n = " << deg5graph.n << endl;
		}

		deg5graph.al = vector<vector<int>> (deg5graph.n);
		for (int u = l; u < n; ++u) {
			if (morph[u] > -1) {
				for (int v : al[u]) {
					if (morph[v] > -1) {
						deg5graph.al[morph[u]].push_back(morph[v]);
						deg5graph.al[morph[v]].push_back(morph[u]);
					}
				}
			}
		}
		
		vector<DFSGraph> comp = deg5graph.partition_biconnected();

		if(DEBUG_MODE) {
			cerr << comp.size() << endl;
		}
		
		for (DFSGraph c : comp) {
			if (DEBUG_MODE) {
				cerr << "is_clique(): " << c.is_clique() << endl;
				cerr << "is_odd_cycle(): " << c.is_odd_cycle() << endl;
			}

			if (!(c.is_clique() || c.is_odd_cycle())) return false;
		}
		
		return true;
	}
	
	bool check_gadget5(int u, int v, int w, int x, int y) {
		if(am[u][v] && am[u][w] && am[u][x] && am[u][y] && am[v][w] && am[w][x] && am[x][y]) {
		//TODO: verify that this is enough
			if(al_adjacent(u, v, w) && al_adjacent(u, w, x) && al_adjacent(u, x, y)) {
				if(al[u].size() <= 5 && al[v].size() <= 5 && al[w].size() <= 6 && al[x].size() <= 6 && al[y].size() <= 5) {
					return true;
				}
			}
		}
		return false;
	}
	
	bool gadget5_test() {
		//TODO: Improve complexity 
		for (int u = l; u < n; ++u) {
			for (int v = l; v < n; ++v) {
				for (int w = l; w < n; ++w) {
					for (int x = l; x < n; ++x) {
						for (int y = l; y < n; ++y) {
							if(check_gadget5(u, v, w, x, y)) return false;
						}
					}
				}
			}
		}
		return true;
	}
	
		
	
	
	
	 
	
};
