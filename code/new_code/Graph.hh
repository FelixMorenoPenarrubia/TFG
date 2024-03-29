#ifndef GRAPH_HH_
#define GRAPH_HH_

#include<iostream>
#include<vector>
#include<map>

struct Graph {
    int n;
    int m;

    std::vector<std::vector<int>> al;
    std::vector<std::map<int, int>> ral;

    // FUNCTIONS

    Graph();

    Graph(std::vector<std::vector<int>> _al);

    bool empty() const;

    bool connected() const;

    bool neighbors(int u, int v) const;

    void generate_ral_and_m();

    void set_starting_edge_in_al(int u, int v);

    void set_outer_face(std::vector<int> of);

    std::vector<int> get_face_ccw(int u, int v) const;

    Graph identify_vertices(std::vector<int> v1, const std::vector<int> v2) const;

    bool subgraph_isomorphism(const Graph& g) const;

    void write_cpp(std::ostream& os) const;

};

#endif