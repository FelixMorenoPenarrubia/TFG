#include "DFSGraph.hh"
#include "debug_utility.hh"

#include<unordered_map>
using std::unordered_map;

DFSGraph::DFSGraph() {
    n = 0;
    al = vector<vector<int>>();
}

DFSGraph::DFSGraph(const Graph& g) {
    n = g.n;
    al = g.al;
}

void DFSGraph::dt_dfs(int v, int par) {
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

	void DFSGraph::dt_dfs_cedges(vector<vector<pair<int, int>>>& cedges, vector<pair<int, int>>& edge_stack, int v, int par) {
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

	void DFSGraph::generate_dfs_tree(int root) {
		dfs_children = vector<vector<int>>(n, vector<int>());
		dfs_parent = vector<int>(n);
		dfs_count = 0;
		dfs_num = vector<int>(n, -1);
		dfs_low = vector<int>(n, -1);
		dt_dfs(root, -1);
	}

	void DFSGraph::generate_edge_partition(vector<vector<pair<int, int>>>& cedges, vector<pair<int, int>>& edge_stack, int root) {
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
	
	vector<DFSGraph> DFSGraph::partition_biconnected() {
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
	
	bool DFSGraph::is_clique() {
		for (int u = 0; u < n; ++u) {
			if ((int)al[u].size() < n-1) return false;
		}
		return true;
	}
	
	bool DFSGraph::is_connected() {
		generate_dfs_tree(0);
		return dfs_count == n;
	}
	
	bool DFSGraph::is_odd_cycle() {
		if (n % 2 == 0) return false;
		for (int u = 0; u < n; ++u) {
			if ((int)al[u].size() != 2) return false;
		}
		if (!is_connected()) return false;
		return true;
	}