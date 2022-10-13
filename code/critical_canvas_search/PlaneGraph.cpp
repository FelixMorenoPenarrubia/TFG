#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<map>
#include<unordered_map>
#include<utility>
#include<functional>
#include<queue>
#include<cstdint>
#include<chrono>

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
	vector<int8_t> code;
	
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

	Code(const string& s) {
		for (int i=0; i < (int)s.length(); ++i) {
			if (s[i] == 'F') code.push_back(-1);
			else if (s[i] == 'B') code.push_back(-2);
			else if ('0' <= s[i] && s[i] <= '9') code.push_back(s[i]-'0');
			else code.push_back(s[i]-'a'+10);
		}
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

//From a sketchy stackoverflow post: https://stackoverflow.com/questions/10405030/c-unordered-map-fail-when-used-with-a-vector-as-key
struct VectorHasher {
    int operator()(const vector<int8_t> &V) const {
        int hash = V.size();
        for(const int8_t &i : V) {
            hash ^= int(i) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};


struct OrientationGraph {
	int n;
	int m;
	vector<pair<int, int> > el; //edge list
	vector<vector<int> > il; // incidence list

	vector<int> base_orientation; //orientation for each edge: +1 first -> second, -1 second -> first


	//constructor providing n, and el, generates il
	OrientationGraph(int _n, vector<pair<int, int> > _el) {
		n = _n;
		el = _el;
		m = el.size();
		il = vector<vector<int>>(n);
		for (int i=0; i < m; ++i) {
			il[el[i].first].push_back(i);
			il[el[i].second].push_back(i);
		}
	}

	//Finds all orientations with indegree bounds bounds, which may have to be satisfied with equality or not, and it also allows 
	//finding only one orientation
	//DEPRECATED - use find_orientation_differences_map for all purposes
	vector<vector<int> > find_orientations(const vector<int>& bounds, bool equality, bool find_one) {
		vector<vector<int > > ans;
		vector<int> curr_ori(m);
		vector<int> curr_bounds = vector<int>(bounds);
		vector<int> curr_rbounds(n);

		for (int i=0; i < n; ++i) {
			curr_rbounds[i] = (int)il[i].size() - curr_bounds[i];
		}

		

		auto change_edge_bounds = [&] (int e, int delta) {
			if (curr_ori[e] == 1) {
				curr_bounds[el[e].second] += delta;
				curr_rbounds[el[e].first] += delta;
			}
			else {
				curr_bounds[el[e].first] += delta;
				curr_rbounds[el[e].second] += delta;
			}
		};

		auto can_change_edge = [&] (int e, int ori) -> bool {
			if (ori == 1) {
				return (curr_bounds[el[e].second] >= 1 && (!equality || (curr_rbounds[el[e].first] >= 1)));
			}
			else {
				return (curr_bounds[el[e].first] >= 1 && (!equality || (curr_rbounds[el[e].second] >= 1)));
			}
		};

		vector<pair<int, int> > decision_stack;
		if (can_change_edge(0, 1)) {
			curr_ori[0] = 1;
			change_edge_bounds(0, -1);
			decision_stack.emplace_back(0, 1);
		}
		else if (can_change_edge(0, -1)) {
			curr_ori[0] = -1;
			change_edge_bounds(0, -1);
			decision_stack.emplace_back(0, 0);
		}
		else {
			return ans;
		}
		
		for (int i=1; i < m; ++i) {
			if (!can_change_edge(i, 1) || !can_change_edge(i, -1)) {
				if (can_change_edge(i, 1)) {
					curr_ori[i] = 1;
					change_edge_bounds(i, -1);
					decision_stack.emplace_back(i, 0);
				}
				else if (can_change_edge(i, -1)) {
					curr_ori[i] = -1;
					change_edge_bounds(i, -1); //note that the second parameter is delta, not orientation...
					decision_stack.emplace_back(i, 0);
				}
			}
		}

		while (!decision_stack.empty()) {

			if(DEBUG_MODE && rand()%1000000 == 0) {
				
				cerr << "Current stack status:" << endl;
				for(pair<int, int> x : decision_stack) {
					cerr << x.first << " " << x.second << endl;
				}
				cerr << endl;
				
			}

			bool decided = false;
			bool all_set = true;
			for (int i=0; i < m; ++i) {
				if (curr_ori[i] == 0) {
					all_set = false;
					if(decided) {
						if (!can_change_edge(i, 1) || !can_change_edge(i, -1)) {
							if (can_change_edge(i, 1)) {
								curr_ori[i] = 1;
								change_edge_bounds(i, -1);
								decision_stack.emplace_back(i, 0);
							}
							else if (can_change_edge(i, -1)) {
								curr_ori[i] = -1;
								change_edge_bounds(i, -1); //note that the second parameter is delta, not orientation...
								decision_stack.emplace_back(i, 0);
							}
						}
					}
					else {
						if (can_change_edge(i, 1)) {
							curr_ori[i] = 1;
							change_edge_bounds(i, -1);
							decision_stack.emplace_back(i, 1);
							decided = true;
						}
						else if (can_change_edge(i, -1)) {
							curr_ori[i] = -1;
							change_edge_bounds(i, -1); //note that the second parameter is delta, not orientation...
							decision_stack.emplace_back(i, 0);
							decided = true;
						}
					}
				}
			}

			if (all_set) {
				ans.push_back(curr_ori);
				if (find_one) return ans;
			}
			if (!decided) {
				bool solved = false;
				while (!solved) {
					while (!decision_stack.empty() && decision_stack.back().second == 0) {
						int edg_id = decision_stack.back().first;
						decision_stack.pop_back();
						change_edge_bounds(edg_id, +1);
						curr_ori[edg_id] = 0;
					}
					if (!decision_stack.empty()) {
						int edg_id = decision_stack.back().first;
						decision_stack.pop_back();
						change_edge_bounds(edg_id, +1);
						if (can_change_edge(edg_id, -curr_ori[edg_id])) {
							curr_ori[edg_id] = -curr_ori[edg_id];
							change_edge_bounds(edg_id, -1);
							decision_stack.emplace_back(edg_id, 0);
							solved = true;
						}
						else {
							curr_ori[edg_id] = 0;
						}
					}
					else {
						solved = true;
					}
				}
			}
		}

		return ans;
	}

	bool find_base_orientation(const vector<int>& bounds) {
		vector<vector<int> > ret = find_orientations(bounds, false, true);
		if (ret.empty()) return false;
		base_orientation = ret[0];
		return true;
	}

	bool find_strict_base_orientation(const vector<int>& bounds) {
		vector<vector<int> > ret = find_orientations(bounds, true, true);
		if (ret.empty()) return false;
		base_orientation = ret[0];
		return true;
	}

	int find_orientation_difference() {
		if(base_orientation.empty()) {
			cerr << "ERROR - did you call find_base_orientation" << endl;
		}

		if (DEBUG_MODE) {
			/*cerr << "Edge list: " << endl;
			for (pair<int, int> e : el) {
				cerr << e.first << "-" << e.second << " ";
			}
			cerr << endl;

			cerr << "Base orientation: " << endl;
			for (int x : base_orientation) {
				if(x == 1) cerr << " ";
				cerr << x << "  ";
				
			}
			cerr << endl;*/
		}

		vector<int> bounds(n, 0);
		for (int i=0; i < m; ++i) {
			if (base_orientation[i] == 1) {
				bounds[el[i].second]++;
			}
			else {
				bounds[el[i].first]++;
			}
		}
		vector<vector<int> > ori = find_orientations(bounds, true, false);
		int ans = 0;
		for (vector<int> co : ori) {

			if (DEBUG_MODE) {
				/*cerr << "Orientation: " << endl;
				for (int x : co) {
					if(x == 1) cerr << " ";
					cerr << x << "  ";
				}
				cerr << endl;*/
			}

			int dif = 0;
			for (int i=0; i < m; ++i) {
				if (co[i] != base_orientation[i]) dif++;
			}
			if (dif%2 == 0) ans++;
			else ans--;
		}
		return ans;
	}

	

	std::unordered_map<vector<int8_t>, int, VectorHasher> find_orientation_differences_map(const vector<int>& bounds) {

		int ori_number = 0;

		std::unordered_map<vector<int8_t>, int, VectorHasher> ans;
		vector<int8_t> indeg(n, 0);
		vector<int> ori(m, -1); //0: first -> second, 1: second -> first
		int parity = 1;
		vector<pair<int, int> > decision_stack; // (e, d), d == 1 if can decide, d == 0 if forced
		auto edge_options = [&] (int e) -> vector<int> {
			vector<int> v;
			if (indeg[el[e].second] < bounds[el[e].second]) v.push_back(0);
			if (indeg[el[e].first] < bounds[el[e].first]) v.push_back(1);
			return v;
		};
		std::function<void(int, int)> set_edge_ori = [&] (int e, int o) -> void {
			if (ori[e] == 0) {
				indeg[el[e].second]--;
			}
			if (ori[e] == 1) {
				indeg[el[e].first]--;
				parity *= -1;
			}
			ori[e] = o;
			if (ori[e] == 0) {
				indeg[el[e].second]++;
			}
			if (ori[e] == 1) {
				indeg[el[e].first]++;
				parity *= -1;
			}

			if (o == 0 || o == 1) {
				for (int i=0; i < m; ++i) {
					if (ori[i] == -1 && edge_options(i).size() == 1) {
						decision_stack.emplace_back(i, 0);
						set_edge_ori(i, edge_options(i)[0]);
					}
				}
			}
		};

		vector<int> degree(n, 0);
		for (auto p : el) {
			degree[p.first]++;
			degree[p.second]++;
		}

		int rnd_val =2343325;
		std::function<int()> my_rand = [&] () -> int {
			rnd_val = (rnd_val*3457+1)%104729;
			return rnd_val;
		};

		auto score_calculation = [&] (int i, int ori) -> int {
			//Least slack (bounds-indeg)
			return (ori ? -(bounds[el[i].first]-indeg[el[i].first]) : -(bounds[el[i].second]-indeg[el[i].second]));
			//Most (degree-bounds)
			//return (ori ? degree[el[i].first]-bounds[el[i].first] : degree[el[i].second]-bounds[el[i].second]);
			//Most degree
			//return (ori ? degree[el[i].first] : degree[el[i].second]);
			//Least degree
			//return (ori ? -degree[el[i].first] : -degree[el[i].second]);
			//Random
			//return my_rand();
		};

		auto choose_edge = [&] () -> pair<int, int> {
			pair<int, int> p (-1, -1);
			int max_score = -2e9;
			for (int i=0; i < m; ++i) {
				if (ori[i] == -1 && !edge_options(i).empty()) {
					bool op0ok = (edge_options(i).size() == 2 || edge_options(i)[0] == 0);
					bool op1ok = (edge_options(i).size() == 2 || edge_options(i)[0] == 1);
					if (op0ok && score_calculation(i, 0) > max_score) {
						max_score = score_calculation(i, 0);
						p = pair<int, int>(i, 0);
					}
					if (op1ok && score_calculation(i, 1) > max_score) {
						max_score = score_calculation(i, 1);
						p = pair<int, int>(i, 1);
					}
				}
			}
			return p;
			
		};


		
		
		/*
		vector<int> opt0 = edge_options(0);
		if (opt0.size() == 2) {
			decision_stack.emplace_back(0, 1); // ALWAYS push to stack before set_edge_ori
			set_edge_ori(0, 0);
		}
		else if (opt0.size() == 1) {
			decision_stack.emplace_back(0, 0);
			set_edge_ori(0, opt0[0]);
		}*/

		pair<int, int> initial_choice = choose_edge();
		if (initial_choice.first == -1) return ans;
		if (edge_options(initial_choice.first).size() == 2) {
			decision_stack.emplace_back(initial_choice.first, 1);
			set_edge_ori(initial_choice.first, initial_choice.second);
		}
		else {
			decision_stack.emplace_back(initial_choice.first, 0);
			set_edge_ori(initial_choice.first, initial_choice.second);
		}

		while (!decision_stack.empty()) {

			bool all_set = true;
			bool blocked = false;
			/*for (int e=0; e < m && all_set; ++e) {
				if (ori[e] == -1) {
					all_set = false;
					if (edge_options(e).empty()) {
						blocked = true;
					}
					else if (edge_options(e).size() == 1) {
						decision_stack.emplace_back(e, 0);
						set_edge_ori(e, edge_options(e)[0]);
					}
					else {
						decision_stack.emplace_back(e, 1);
						set_edge_ori(e, 0);
					}
				}
			}*/

			for (int e = 0; e < m && !blocked; ++e) {
				if (ori[e] == -1) {
					all_set = false;
					if (edge_options(e).empty()) {
						blocked = true;
					}
				}
			}

			if (all_set) {
				ans[indeg] += parity;

				ori_number++;
			}
			if (all_set || blocked) {
				while (!decision_stack.empty()) {
					pair<int, int> p = decision_stack.back();
					decision_stack.pop_back();
					if (p.second) {
						decision_stack.emplace_back(p.first, 0);
						set_edge_ori(p.first, 1-ori[p.first]);
						break;
					}
					else {
						set_edge_ori(p.first, -1);
					}
				}
			}
			else {
				pair<int, int> choice = choose_edge();
				decision_stack.emplace_back(choice.first, 1);
				set_edge_ori(choice.first, choice.second);
			}
		}

		if (ori_number > 1e7) {
			cerr << "Orientation number: " << ori_number << endl;
			cerr << "Ans map size: " << ans.size() << endl;
		}


		return ans;
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
	//vector< vector <int> > am; //adjacency matrix representation
	
	
	PlaneGraph(int _l, vector< vector <int> > _al) {
		l = _l;
		al = _al;
		n = al.size();
		//am = vector< vector<int> >(n, vector<int>(n, 0));
		ral = vector< std::map<int, int> > (n);
		m = 0;
		for (int u = 0; u < n; ++u) {
			for (int i = 0; i < (int)al[u].size(); ++i) {
				int v = al[u][i];
				ral[u][v] = i; 
				//am[u][v] = 1;
				m += 1;
			}
		}
		m /= 2;
	}

	PlaneGraph(const Code& code) {
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
				//am[u][v] = 1;
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
	
	//Note: may give different codes for isomorphic graphs if the outer orientation is reversed 
	Code compute_code() {
		Code code = std::min(compute_code_edge(0, 1), compute_code_edge(1, 0));
		for (int u = 1; u < l; ++u) {
			code = std::min(code, std::min(compute_code_edge(u, (u+1)%l), compute_code_edge((u+1)%l, u)));
		}
		return code;
	}

	//Returns number of interior edges
	int interior_edges() {
		int ans = 0;
		for (int u = l; u < n; ++u) {
			for (int v : al[u]) {
				if (v > u) ans++;
			}
		}
		return ans;
	}
	

	//Adds tripod: vertex j from the outer face is pushed into inner face
	//and adds s >= 1 outer vertices
	//connected to j depending on whether the bit in bitmask bm is set
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


	//Fuses graphs g1 and g2 along the edges j1->j1+1 and j2->j2 +- 1 of the outer face
	//if same_orientation is false, the orientation of the second graph is reversed
	static PlaneGraph fuse_chord(PlaneGraph g1, PlaneGraph g2, int j1, int j2, bool same_orientation) {
		

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

				if (DEBUG_MODE) {
					//cerr << morph1[u] << endl;
				}

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

		PlaneGraph res = PlaneGraph(g1.l+g2.l-2, nal);

		if (DEBUG_MODE) {
			cerr << "fuse_chord(" << g1.compute_code().to_string() << ", " << g2.compute_code().to_string() << ", " 
			<< j1 << ", " << j2 << ", " << same_orientation << ")" << endl;
			cerr << "Result = " << res.compute_code().to_string() << endl;
			//cerr << g1.n << " " << g2.n << endl;
			//cerr << g1.l << " " << g2.l << endl;
		}

		return res;
	}

	//Returns new graph with interior vertex v removed
	PlaneGraph remove_vertex(int v) {
		vector<int> morph(n);
		for (int i = 0; i < n; ++i) {
			morph[i] = i - int(i > v);
		}
		vector<vector<int> > nal(n-1);
		for (int u = 0; u < n; ++u) {
			if (u == v) continue;
			for (int w : al[u]) {
				if (w == v) continue;
				nal[morph[u]].push_back(morph[w]);
			}
		}
		return PlaneGraph(l, nal);
	}


	//Returns the subgraph contained in the interior of the cycle given
	//Orientation matters for interior/exterior
	PlaneGraph subgraph_contained_in_cycle(vector<int> cyc) {
		vector<int> morph(n, -1);
		int cs = cyc.size();
		for (int i = 0; i < cs; ++i) {
			morph[cyc[i]] = i;
		}
		int cn = cs;
		std::queue<int> q;
		for (int i = 0; i < cs; ++i) {
			int u = cyc[i];
			int j = ral[u][cyc[(i+1)%cs]]+1;
			j %= al[u].size();
			while (al[u][j] != cyc[(i-1+cs)%cs]) {
				int v = al[u][j];
				if (morph[v] == -1) {
					morph[v] = cn++;
					q.push(v);
				}
				j++;
				j %= al[u].size();
			}
		}
		while (!q.empty()) {
			int u = q.front();
			q.pop();
			for (int v : al[u]) {
				if (morph[v] == -1) {
					morph[v] = cn++;
					q.push(v);
				}
			}
		}

		vector<vector<int> > nal(cn);

		for (int u = 0; u < n; ++u) {
			if (morph[u] == -1) continue;
			for (int v : al[u]) {
				if (morph[v] == -1) continue;
				nal[morph[u]].push_back(morph[v]);
			}
		}

		return PlaneGraph(cs, nal);

	}


	//Returns the subgraph contained by the cycle v-a-...-b-v
	//Note: v-a-...-b-v must be positively oriented
	PlaneGraph partition_outer_face_interval(int v, int a, int b) {
		vector<int> cyc;
		cyc.push_back(v);
		for (int i=a; i % l != b; ++i) {
			cyc.push_back(i%l);
		}
		cyc.push_back(b);
		return subgraph_contained_in_cycle(cyc);
	}

	//Returns a vector of new graphs resulting from partitioning the graph along the neighbours of the outer
	//face of interior vertex v
	vector<PlaneGraph> partition_along_vertex(int v) {
		vector<int> outer_face_neighbours;
		for (int w : al[v]) {
			if (w < l) outer_face_neighbours.push_back(w);
		}
		vector<PlaneGraph> ans;
		int m = outer_face_neighbours.size();

		for (int i=0; i < m; ++i) {
			ans.push_back(partition_outer_face_interval(v, outer_face_neighbours[i], outer_face_neighbours[(i+1)%m]));
		}
		return ans;
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
	
	//now basically useless - and uses deprecated am
	/*bool check_gadget4(int u, int v, int w, int x) {
		if(am[u][v] && am[u][w] && am[u][x] && am[v][w] && am[w][x] && x != v) {
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
	}*/
	
	bool biconnected_deg5_components_test() {

		

		vector<int> morph(n, -1);
		DFSGraph deg5graph;
		deg5graph.n = 0;
		for (int u = l; u < n; ++u) {
			int deg = al[u].size();
			if (deg == 5) {
				morph[u] = deg5graph.n++;
			}
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
			//cerr << comp.size() << endl;
		}
		
		for (DFSGraph c : comp) {
			/*if (DEBUG_MODE) {
				cerr << "is_clique(): " << c.is_clique() << endl;
				cerr << "is_odd_cycle(): " << c.is_odd_cycle() << endl;
			}*/

			if (!(c.is_clique() || c.is_odd_cycle())) return false;
		}
		
		return true;
	}
	
	bool neighbour(int u, int v) {
		return ral[u].find(v) != ral[u].end();
	}
	
	bool gadget4_test() {
		for (int u = l; u < n; ++u) {
			if (al[u].size() <= 5) {
				for (int v : al[u]) {
					if (v >= l && al[v].size() <= 5) {
						for (int w : al[u]) {
							if (w >= l && neighbour(v, w) && al[w].size() <= 6) {
								for (int x : al[u]) {
									if (x >= l && neighbour(x, w) && x != v && al[x].size() <= 5) {
										return false;
									}
								}
							}
						}
					}
				}
			}
		}
		return true;
	}
	bool gadget5_test() {
		for (int u = l; u < n; ++u) {
			if (al[u].size() <= 5) {
				for (int v : al[u]) {
					if (v >= l && al[v].size() <= 5) {
						for (int w : al[u]) {
							if (w >= l && neighbour(v, w) && al[w].size() <= 6) {
								for (int x : al[u]) {
									if (x >= l && neighbour(w, x) && x != v && al[x].size() <= 6) {
										for (int y : al[u]) {
											if (y >= l && neighbour(x, y) && y != w && y != v && al[y].size() <= 5) {
												return false;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		return true;
	}

	bool weak_alon_tarsi_test() {
		vector<pair<int, int> > el;
		vector<int> bounds (n-l); 
		for (int u = l; u < n; ++u) {
			bounds[u-l] = 4;
			for (int v : al[u]) {
				if (v > u) {
					el.emplace_back(u-l, v-l);
				}
				else if (v < l) {
					bounds[u-l]--;
				}
			}
		}
		if (el.empty()) return true;
		OrientationGraph og = OrientationGraph(n-l, el);
		if (!og.find_base_orientation(bounds)) return true;

		return og.find_orientation_difference() == 0;
	}

	static void bounds_backtracking(int i, int sum, int bounds_sum, vector<int>& curr_bounds, const vector<int>& bounds, vector<vector<int>>& ans) {
		int n = bounds.size();
		if (i == n) {
			if (sum == 0) {
				ans.push_back(vector<int>(curr_bounds));
			}
		}
		else {
			for (int v = std::max(0, sum-bounds_sum+bounds[i]); v <= std::min(bounds[i], sum); ++v) {
				curr_bounds[i] = v;
				bounds_backtracking(i+1, sum-v, bounds_sum-bounds[i], curr_bounds, bounds, ans);
			}
		}
	}

	static vector<vector<int> > bounds_generation(const vector<int>& bounds, int m) {
		int n = bounds.size();
		int bounds_sum = 0;
		for (int x : bounds) {
			bounds_sum += x;
		}
		vector<vector<int> > ans;
		vector<int> curr_bounds(n, 0);
		bounds_backtracking(0, m, bounds_sum, curr_bounds, bounds, ans);
		return ans;
	}

	bool alon_tarsi_always_colorable() {

		/*if (DEBUG_MODE) {
			cerr << "Doing Alon-Tarsi test, interior vertices: " << n-l << endl;
		}*/


		vector<pair<int, int> > el;
		vector<int> bounds (n-l); 
		for (int u = l; u < n; ++u) {
			bounds[u-l] = 4;
			for (int v : al[u]) {
				if (v > u) {
					el.emplace_back(u-l, v-l);
				}
				else if (v < l) {
					bounds[u-l]--;
					if (bounds[u-l] == -1) return false;
				}
			}
		}
		if (el.empty()) return true;

		

		OrientationGraph og = OrientationGraph(n-l, el);
		/*
		vector<vector <int> > bounds_vector = bounds_generation(bounds, el.size());
		for (vector<int> strict_bounds : bounds_vector) {
			if (og.find_strict_base_orientation(strict_bounds)) {
				if (og.find_orientation_difference() != 0) return true;
			}
		}
		*/

		/*if (DEBUG_MODE) {
			cerr << "Going into the implementation" << endl;
		}*/

		//if (DEBUG_MODE) {

			/*std::chrono::steady_clock::time_point begin;
			if (el.size() > 25) {
				cerr << "Alon-Tarsi test on graph with " << el.size() << " edges" << endl;
				cerr << compute_code().to_string() << endl;
				begin = std::chrono::steady_clock::now();
			}*/
		//}

		std::unordered_map<vector<int8_t>, int, VectorHasher> diffs = og.find_orientation_differences_map(bounds);

		//if (DEBUG_MODE) {
			/*if (el.size() > 25) {
				std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
				cerr << "Finished" << endl;
				cerr << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << endl;
			}*/
		//}

		for (auto x : diffs) {
			if (x.second != 0) return true;
		}
		return false;
	}

	bool strong_alon_tarsi_test() {
		bool edges_inside = false;
		for (int u = l; u < n; ++u) {
			for (int v : al[u]) {
				if (v >= l) {
					edges_inside = true;
				}
			}
		}
		if (!edges_inside) return true;
		

		return !alon_tarsi_always_colorable();
	}

	//Returns the vertices that have to be deleted in order to obtain the minimal graph which passes
	//the Alon-Tarsi test
	//NOTE: this is a bit of a ugly hack due to the inability of arbitrarily label vertices in subgraphs,
	//maybe take into account if I ever refactor the code
	vector<int> minimal_irreducible_deletedvertices() {
		/*if (DEBUG_MODE) {
			cerr << "minimal_irreducible_deletedvertices " << n << " " << l << endl;
		}*/

		if (n <= l+1) return vector<int>();

		/*if (DEBUG_MODE) {
			cerr << "minimal_irreducible_deletedvertices2" << endl;
		}*/
		for (int v = l; v < n; ++v) {
			PlaneGraph g = remove_vertex(v);
			if(!g.alon_tarsi_always_colorable()) {
				vector<int> dv = g.minimal_irreducible_deletedvertices();
				for (int i=0; i < (int)dv.size(); ++i) {
					if(dv[i] >= v) dv[i]++;
				}
				dv.push_back(v);
				return dv;
			}
		}
		return vector<int>();
	}


	static std::map<Code, bool> rr_aa_mem;

	int outer_face_neighbours_num(int v) {
		int ans = 0;
		for (int w : al[v]) {
			if (w < l) ans++;
		}
		return ans;
	}

	//TODO: ensure that there are no weird cases when alon tarsi test passes when it should not
	//TODO: currently, it does not really work with full generality - it can only erase vertices with at least two outer neighbours
	bool recursive_reducibility_alon_tarsi_test() {
		if (n == l) return true;

		Code code = compute_code();
		if (rr_aa_mem.find(code) != rr_aa_mem.end()) {
			return rr_aa_mem[code];
		}

		if (!strong_alon_tarsi_test()) return rr_aa_mem[code] = false;
		vector<int> deleted_vertices = minimal_irreducible_deletedvertices();	

		/*if (DEBUG_MODE) {
			cerr << "Deleted vertices: ";
			for (int v : deleted_vertices) cerr << v << " ";
			cerr << endl;
		}*/

		vector<int> deleted(n);
		for (int v : deleted_vertices) deleted[v] = 1;
		int undel_vertex = l;
		while (deleted[undel_vertex] || outer_face_neighbours_num(undel_vertex) < 2) undel_vertex++;
		vector<PlaneGraph> vhpp = partition_along_vertex(undel_vertex);
		for (PlaneGraph g : vhpp) {

			/*if (DEBUG_MODE) {
				cerr << "Recursively going into " << g.n << " " << g.l << endl;
				for (int u=0; u < g.n; ++u) {
					for (int v : g.al[u]) {
						cerr << u << " " << v << endl;
					}
				}
			}*/

			if (!g.recursive_reducibility_alon_tarsi_test()) return rr_aa_mem[code] = false;
		}
		return rr_aa_mem[code] = true;
	}

	bool old_alon_tarsi_test() {
		vector<pair<int, int> > el;
		vector<int> bounds (n-l); 
		for (int u = l; u < n; ++u) {
			bounds[u-l] = 4;
			for (int v : al[u]) {
				if (v > u) {
					el.emplace_back(u-l, v-l);
				}
				else if (v < l) {
					bounds[u-l]--;
				}
			}
		}
		if (el.empty()) return true;
		OrientationGraph og = OrientationGraph(n-l, el);
		vector<vector <int> > bounds_vector = bounds_generation(bounds, el.size());
		for (vector<int> strict_bounds : bounds_vector) {
			if (og.find_strict_base_orientation(strict_bounds)) {
				if (og.find_orientation_difference() != 0) return false;
			}
		}

		return true;
	}

	
};

std::map<Code, bool> PlaneGraph::rr_aa_mem;
