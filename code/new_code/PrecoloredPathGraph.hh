#ifndef PRECOLOREDPATHGRAPH_HH_
#define PRECOLOREDPATHGRAPH_HH_

#include "PrecoloredGraph.hh"

#include<iostream>
#include<vector>
#include<map>
#include<algorithm>
#include<string>



struct PrecoloredPathGraphCode {
    static const int F_OFFSET = -100;
    std::vector<int> code;

    PrecoloredPathGraphCode();

    PrecoloredPathGraphCode(const std::string& s);

    void push_r(int x);
    
    void push_f(int list_size);

    void push_b();

    int size() const;

    std::string to_string() const;

    int operator[](int i) const;

    bool operator<(const PrecoloredPathGraphCode& b) const;

};

struct PrecoloredPathGraph : public PrecoloredGraph {


    int l;


    // FUNCTIONS

    void init_inherited_values();

    PrecoloredPathGraph();

    PrecoloredPathGraph(std::vector<std::vector<int>> _al, int _l, std::vector<int> _list_sizes);

    PrecoloredPathGraph(const PrecoloredPathGraphCode& code);

    void write(std::ostream& os) const;

    void write_prolog(std::ostream& os) const;

    static PrecoloredPathGraph read(std::istream& is);
    
    static PrecoloredPathGraph read_code(std::istream &is);

    static PrecoloredPathGraph empty_cycle(int l, std::vector<int> ls);
    
    static PrecoloredPathGraph empty_path(int l);

    PrecoloredPathGraph add_tripod(int s, int j, long long bm) const;

    PrecoloredPathGraph extend_path_chord(int s, int j) const;

    PrecoloredPathGraph reverse() const;
    
    static PrecoloredPathGraph fuse_chord(const PrecoloredPathGraph& g1, const PrecoloredPathGraph& g2, int list_size_joint);

    static PrecoloredPathGraph fuse_articulation_point(const PrecoloredPathGraph& g1, const PrecoloredPathGraph& g2);

    void dfs_code(int u, int idx, int& c, vector<int>& an, PrecoloredPathGraphCode& code) const;

    PrecoloredPathGraphCode compute_code_edge(int u, int v) const;

    PrecoloredPathGraphCode compute_code() const;

    bool is_interior_path_vertex_articulation_point(int u) const;

    bool test_no_l3_adjacent() const;

    bool test_criticality() const;


    //Returns largest size of a "fan" subgraph with hinge in one of the vertices adjacent
    //to the endpoint of the path (i.e. vertices 0 or l-2)
    int fan_length_in_endpoints() const;

    //Returns largest size of a "fan" subgraph with hinge in one of the interior vertices of the path
    int fan_length_in_path() const;

    //Returns true if there is a bellows with hinge in one of the vertices adjacent to
    //the endpoint of the path and the corresponding endpoint precolored
    bool has_bellows_in_endpoints() const;

    //Returns true if there is a large fan or a bellows at the endpoints
    bool has_large_fans_or_bellows() const;

    //Returns the largest 2-wedge made from a chord incident with vertex 1
    PrecoloredPathGraph largest_left_endpoint_cut() const;

    //Returns the largest 2-wedge made from a chord incident with vertex l-1
    PrecoloredPathGraph largest_right_endpoint_cut() const;

    std::pair<PrecoloredPathGraph, PrecoloredPathGraph> largest_cuts(int u) const;
};

#endif