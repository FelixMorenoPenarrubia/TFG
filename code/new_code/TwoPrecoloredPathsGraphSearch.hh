#ifndef TWOPRECOLOREDPATHSGRAPHSEARCH_HH_
#define TWOPRECOLOREDPATHSGRAPHSEARCH_HH_

#include "TwoPrecoloredPathsGraph.hh"
#include "PrecoloredPathGraphSearch.hh"

#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<queue>
#include<algorithm>
#include<unordered_set>






struct TwoPrecoloredPathsGraphSearch {
    
    std::vector<std::set<TwoPrecoloredPathsGraphCode>> critical;
    PrecoloredPathGraphSearch ppsearch;

    TwoPrecoloredPathsGraphSearch();

    static void add_graph(const TwoPrecoloredPathsGraph& g, std::set<TwoPrecoloredPathsGraphCode>& gl, int d);

    static void add_graphs(const vector<TwoPrecoloredPathsGraph>& vg, std::set<TwoPrecoloredPathsGraphCode>& gl, int d);

    static bool test_graph(const TwoPrecoloredPathsGraph& g, int d);

    void add_same_side(int d, std::set<TwoPrecoloredPathsGraphCode>& gl);

    void add_opposite_side(int d, std::set<TwoPrecoloredPathsGraphCode>& gl);

    std::set<TwoPrecoloredPathsGraphCode> generate(int d);

    vector<TwoPrecoloredPathsGraph> get(int d);

    std::set<TwoPrecoloredPathsGraphCode> get_codes(int d);

};

#endif