#ifndef LISTGRAPH_HH_
#define LISTGRAPH_HH_

#include "Graph.hh"

#include<iostream>
#include<vector>
#include<map>
#include<algorithm>




struct ListGraphCode {
	std::vector<int> code;
    static const int F_OFFSET = -100;
	
	ListGraphCode() {
	
	}
	
	void push_r(int x);
	
	void push_f(int list_size);
	
	void push_b();
	
	int size() const;
	
	std::string to_string() const;
	int operator[](int i) const;

	bool operator<(const ListGraphCode& b) const;
};




struct ListGraph : public Graph {
    
    std::vector<int> list_sizes;
    

    // FUNCTIONS

    ListGraph();

    ListGraph(std::vector<std::vector<int>> _al, std::vector<int> _list_sizes);

    ListGraph remove_vertex(int w) const;

    ListGraph precolor_vertex(int w) const;

    bool nocolors() const;

    void dfs_code(int u, int idx, int& c, std::vector<int>& an, ListGraphCode& code) const;

    ListGraphCode compute_list_code_edge(int u, int v) const;

    ListGraphCode compute_list_code() const;

    void dfs_cc (int u, std::vector<int>& visited, std::vector<int>& vis_stack) const;

    ListGraph degree_assignment_subgraph() const;

    std::vector<ListGraph> connected_components() const;

};

#endif