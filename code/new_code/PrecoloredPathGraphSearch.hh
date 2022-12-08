#ifndef PRECOLOREDPATHGRAPHSEARCH_HH_
#define PRECOLOREDPATHGRAPHSEARCH_HH_

#include "PrecoloredPathGraph.hh"

#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<queue>
#include<algorithm>
#include<unordered_set>





using GraphList = std::set<PrecoloredPathGraphCode>;

struct PrecoloredPathGraphSearch {
    
    std::vector<GraphList> critical_biconnected;
    std::vector<GraphList> critical_all;

    PrecoloredPathGraphSearch();

    static void add_graph(const PrecoloredPathGraph& g, GraphList& cl);

    static void add_graph_q_and_cl(const PrecoloredPathGraph& g, GraphList& cl, std::queue<PrecoloredPathGraphCode>& q);

    static bool test_graph(const PrecoloredPathGraph& g);

    void add_smaller_articulation_points(int l, const std::vector<GraphList>& prev, GraphList& curr) const;

    void add_smaller_crossing_chords(int l, const std::vector<GraphList>& prev, GraphList& curr) const;

    void add_smaller_path_chords(int l, const std::vector<GraphList>& prev, GraphList& curr) const;

    void add_smaller_tripods(int l, const std::vector<GraphList>& prev, GraphList& curr) const;

    void add_same_size_tripods_and_chords(int l, GraphList& curr) const;

    
    GraphList generate_biconnected(int l) const;

    GraphList generate_all(int l) const;

    vector<PrecoloredPathGraph> get_all(int l);

    GraphList get_all_codes(int l);

};

#endif