#include<vector>
#include<map>
#include<iostream>
#include "Code.h"

#ifndef GRAPH_H_
#define GRAPH_H_

using std::vector;



struct Graph {
    int n;
    int m;

    vector<int> list_sizes;
    vector<vector <int> > al;
    vector<std::map<int, int> > ral;

    //FUNCTIONS

    Graph();

    Graph(vector<int> _list_sizes, vector<vector<int> > _al);

    bool empty() const;
    
    bool nocolors() const;

    static Graph read(std::istream& is);

    void write(std::ostream& os) const;
    
    void dfs_code(int u, int idx, int& c, vector<int>& an, Code& code) const;

    Code compute_code_edge(int u, int v) const;

    Code compute_code() const;

    Graph remove_vertex(int w) const;

    Graph precolor_vertex(int w) const;

    void dfs_cc (int u, vector<int>& visited, vector<int>& vis_stack) const;

    vector<Graph> connected_components() const;


};



#endif