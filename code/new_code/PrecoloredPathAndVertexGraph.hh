#ifndef PRECOLOREDPATHANDVERTEXGRAPH_HH_
#define PRECOLOREDPATHANDVERTEXGRAPH_HH_

#include "PrecoloredGraph.hh"
#include "PrecoloredPathGraph.hh"

#include<iostream>
#include<vector>
#include<map>
#include<algorithm>
#include<string>



struct PrecoloredPathAndVertexGraphCode {
    static const int F_OFFSET = -100;
    std::vector<int> code;

    PrecoloredPathAndVertexGraphCode();

    PrecoloredPathAndVertexGraphCode(const std::string& s);

    void push_r(int x);
    
    void push_f(int list_size);

    void push_b();

    int size() const;

    std::string to_string() const;

    int operator[](int i) const;

    bool operator<(const PrecoloredPathAndVertexGraphCode& b) const;

};

struct PrecoloredPathAndVertexGraph : public PrecoloredGraph {

    void init_inherited_values();

    PrecoloredPathAndVertexGraph();

    PrecoloredPathAndVertexGraph(std::vector<std::vector<int>> _al, std::vector<int> _list_sizes);

    PrecoloredPathAndVertexGraph(const PrecoloredPathAndVertexGraphCode& code);

    void write(std::ostream& os) const;

    void write_prolog(std::ostream& os) const;

    static PrecoloredPathAndVertexGraph read(std::istream& is);
    
    static PrecoloredPathAndVertexGraph read_code(std::istream &is);

    static std::vector<PrecoloredPathAndVertexGraph> fuse_paths(const PrecoloredPathAndVertexGraph& g1, const PrecoloredPathAndVertexGraph& g2);

    static PrecoloredPathAndVertexGraph fuse_vertex(const PrecoloredPathAndVertexGraph&g1, const PrecoloredPathAndVertexGraph& g2, int uls, bool reverse);

    vector<vector<int>> get_paths() const;

    void dfs_code(int u, int idx, int& c, vector<int>& an, PrecoloredPathAndVertexGraphCode& code) const;

    PrecoloredPathAndVertexGraphCode compute_code_edge(int u, int v) const;

    PrecoloredPathAndVertexGraphCode compute_code() const;

    int distance_between_paths() const;

    bool test_no_l3_adjacent() const;

    bool test_criticality() const;

    PrecoloredPathAndVertexGraph reverse() const;

};

#endif