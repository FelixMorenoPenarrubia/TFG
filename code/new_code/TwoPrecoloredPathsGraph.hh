#ifndef TWOPRECOLOREDPATHSGRAPH_HH_
#define TWOPRECOLOREDPATHSGRAPH_HH_

#include "PrecoloredGraph.hh"
#include "PrecoloredPathGraph.hh"
#include "PrecoloredPathAndVertexGraph.hh"

#include<iostream>
#include<vector>
#include<map>
#include<algorithm>
#include<string>



struct TwoPrecoloredPathsGraphCode {
    static const int F_OFFSET = -100;
    std::vector<int> code;

    TwoPrecoloredPathsGraphCode();

    TwoPrecoloredPathsGraphCode(const std::string& s);

    void push_r(int x);
    
    void push_f(int list_size);

    void push_b();

    int size() const;

    std::string to_string() const;

    int operator[](int i) const;

    bool operator<(const TwoPrecoloredPathsGraphCode& b) const;

};

struct TwoPrecoloredPathsGraph : public PrecoloredGraph {

    void init_inherited_values();

    TwoPrecoloredPathsGraph();

    TwoPrecoloredPathsGraph(std::vector<std::vector<int>> _al, std::vector<int> _list_sizes);

    TwoPrecoloredPathsGraph(const TwoPrecoloredPathsGraphCode& code);

    void write(std::ostream& os) const;

    void write_prolog(std::ostream& os) const;

    static TwoPrecoloredPathsGraph read(std::istream& is);
    
    static TwoPrecoloredPathsGraph read_code(std::istream &is);

    static std::vector<TwoPrecoloredPathsGraph> fuse_precoloredpaths_sameside(const PrecoloredPathGraph& g1, const PrecoloredPathGraph& g2);

    static std::vector<TwoPrecoloredPathsGraph> fuse_precoloredpaths_oppositeside(const PrecoloredPathGraph& g1, const PrecoloredPathGraph& g2);

    static std::vector<TwoPrecoloredPathsGraph> fuse_paths(const TwoPrecoloredPathsGraph& g1, const TwoPrecoloredPathsGraph& g2);

    static TwoPrecoloredPathsGraph fuse_edge(const TwoPrecoloredPathsGraph&g1, int u1, int v1, const TwoPrecoloredPathsGraph& g2, int u2, int v2, int uls, int vls, bool reverse);

    vector<vector<int>> get_paths() const;

    void dfs_code(int u, int idx, int& c, vector<int>& an, TwoPrecoloredPathsGraphCode& code) const;

    TwoPrecoloredPathsGraphCode compute_code_edge(int u, int v) const;

    TwoPrecoloredPathsGraphCode compute_code() const;

    int distance_between_paths() const;

    bool test_no_l3_adjacent() const;
    
    int count_l3_adjacent_pairs() const;

    bool test_criticality() const;

    TwoPrecoloredPathsGraph reverse() const;

    std::vector<PrecoloredPathAndVertexGraph> unprecolor_vertices() const;

};

#endif