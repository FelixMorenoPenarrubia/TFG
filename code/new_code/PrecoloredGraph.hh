#ifndef PRECOLOREDGRAPH_HH_
#define PRECOLOREDGRAPH_HH_

#include "ListGraph.hh"

#include<iostream>
#include<vector>
#include<map>
#include<algorithm>


using std::vector;
using std::map;

struct PrecoloredGraph : public ListGraph {
    
    vector<int> precolored;

    // FUNCTIONS

    PrecoloredGraph();

    PrecoloredGraph(vector<vector<int>> _al, vector<int> _precolored);

    //Computes graph with list sizes resulting of removing precolored part
    ListGraph compute_list_graph() const;

};

#endif