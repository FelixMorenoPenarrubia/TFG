#ifndef DFSGRAPH_HH_
#define DFSGRAPH_HH_

#include "Graph.hh"

#include<iostream>
#include<vector>
#include<map>
#include<algorithm>
#include<utility>

//todo: change this, using should not be in hh files
using std::vector;
using std::pair;

struct DFSGraph : public Graph {
    int n;
	
	vector<vector<int> > al;

	vector<vector<int> > dfs_children;
	vector<int> dfs_parent;
	vector<int> dfs_num;
	vector<int> dfs_low;
	int dfs_count;
    bool is_root_ac = false;

    DFSGraph();
    DFSGraph(const Graph& g);
    
    void dt_dfs(int v, int par);
    void dt_dfs_cedges(vector<vector<pair<int, int>>>& cedges, vector<pair<int, int>>& edge_stack, int v, int par);
    void generate_edge_partition(vector<vector<pair<int, int>>>& cedges, vector<pair<int, int>>& edge_stack, int root);
    void generate_dfs_tree(int root);
    vector<DFSGraph> partition_biconnected();
    bool is_odd_cycle();
    bool is_connected();
    bool is_clique();
};

#endif