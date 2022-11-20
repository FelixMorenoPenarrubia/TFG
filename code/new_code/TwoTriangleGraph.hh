#ifndef TWOTRIANGLEGRAPH_HH_
#define TWOTRIANGLEGRAPH_HH_

#include "PrecoloredGraph.hh"
#include "Canvas.hh"

#include<iostream>
#include<vector>
#include<map>
#include<algorithm>
#include<string>



struct TwoTriangleGraphCode {
    std::vector<int> code;

    TwoTriangleGraphCode();

    TwoTriangleGraphCode(const std::string& s);

    void push(int x);
    
    void push_f(bool precolored);

    void push_b();

    int size() const;

    std::string to_string() const;

    int operator[](int i) const;

    bool operator<(const TwoTriangleGraphCode& b) const;


};

struct TwoTriangleGraph : public PrecoloredGraph {


    int path_length;

    std::vector<std::vector<int>> triangles;

    Canvas original_canvas;


    // FUNCTIONS

    void init_inherited_values();

    TwoTriangleGraph();

    TwoTriangleGraph(std::vector<std::vector<int>> _al, std::vector<std::vector<int>> _triangles);

    TwoTriangleGraph(const Canvas& g, int idx);

    TwoTriangleGraph(const TwoTriangleGraphCode& code);

    void set_list_sizes();

    static std::vector<TwoTriangleGraph> generate_from_canvas(const Canvas& g);

    static TwoTriangleGraph read(std::istream& is);

    void write(std::ostream& os) const;

    void write_prolog(std::ostream& os) const;

    void dfs_code(int u, int idx, int& c, vector<int>& an, TwoTriangleGraphCode& code) const;

    TwoTriangleGraphCode compute_code_edge(int u, int v) const;

    TwoTriangleGraphCode compute_code() const;

    int distance_between_triangles() const;

    bool test_criticality() const;

};

#endif