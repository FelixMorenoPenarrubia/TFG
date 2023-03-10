#ifndef PRECOLOREDPATHANDTRIANGLEGRAPH_HH_
#define PRECOLOREDPATHANDTRIANGLEGRAPH_HH_

#include "PrecoloredGraph.hh"
#include "PrecoloredPathGraph.hh"
#include "PrecoloredPathAndVertexGraph.hh"
#include "Canvas.hh"

#include<iostream>
#include<vector>
#include<map>
#include<algorithm>
#include<string>



struct PrecoloredPathAndTriangleGraphCode {
    static const int F_OFFSET = -100;
    std::vector<int> code;

    PrecoloredPathAndTriangleGraphCode();

    PrecoloredPathAndTriangleGraphCode(const std::string& s);

    void push_r(int x);
    
    void push_f(int list_size);

    void push_b();

    int size() const;

    std::string to_string() const;

    int operator[](int i) const;

    bool operator<(const PrecoloredPathAndTriangleGraphCode& b) const;

};

struct CanvasWithIndices {
    int c_idx;
    int i;
    int dj;
    int mask;
};

struct PrecoloredPathAndTriangleGraph : public PrecoloredGraph {

    std::vector<int> path;

    std::vector<int> triangle;

    void init_inherited_values();


    //Initializes path and triangle variables assuming the path is not adjacent to the triangle
    void generate_path_and_triangle();

    PrecoloredPathAndTriangleGraph();

    PrecoloredPathAndTriangleGraph(std::vector<std::vector<int>> _al, std::vector<int> _list_sizes);

    PrecoloredPathAndTriangleGraph(const PrecoloredPathAndTriangleGraphCode& code);

    void write(std::ostream& os) const;

    static PrecoloredPathAndTriangleGraph read(std::istream& is);
    
    static PrecoloredPathAndTriangleGraph read_code(std::istream& is);

    //Returns all graphs in which the Dvorak reduction fails resulting from fusing canvas and path-critical 
    //with two edges of the triangle inside the path
    static std::vector<PrecoloredPathAndTriangleGraph> fuse_canvas_and_path_triangle_in_path(const Canvas& c, const PrecoloredPathGraph& p);

    //Returns all graphs in which the Dvorak reduction fails resulting from fusing canvas and path-critical 
    //with two edges of the triangle inside the canvas
    static std::vector<PrecoloredPathAndTriangleGraph> fuse_canvas_and_path_triangle_in_canvas(const Canvas& c, const PrecoloredPathGraph& p);

    //Fuses canvas and path-critical graph with critical path of length 1 starting at index i of the outer face of the canvas
    //and fusion with path-critical graph starting at index j of the o.f. of the canvas
    //Important: list sizes are left as-is, and the indices of the corresponding vertices of the canvas are not changed
    static PrecoloredPathAndTriangleGraph fuse_canvas_and_path_triangle_in_path_indices(const Canvas& c, const PrecoloredPathGraph& p, int i, int j);

    //Fuses canvas and path-critical graph with critical path of length 1 starting at index i of the outer face of the canvas
    //and fusion with path-critical graph starting at index j of the o.f. of the canvas
    //Important: list sizes are left as-is, and the indices of the corresponding vertices of the canvas are not changed
    static PrecoloredPathAndTriangleGraph fuse_canvas_and_path_triangle_in_canvas_indices(const Canvas& c, const PrecoloredPathGraph& p, int i, int j);

    static vector<PrecoloredPathAndTriangleGraph> fuse_canvas_with_indices_and_path_triangle_in_path(const Canvas& c, const CanvasWithIndices& cwi, const PrecoloredPathGraph& p);

    static vector<PrecoloredPathAndTriangleGraph> fuse_canvas_with_indices_and_path_triangle_in_canvas(const Canvas& c, const CanvasWithIndices& cwi, const PrecoloredPathGraph& p);

    //Returns all CanvasWithIndices that are critical with the modified list sizes and precoloring the fusion path
    static vector<CanvasWithIndices> critical_canvases_with_indices_triangle_in_path(Canvas c);

    static vector<CanvasWithIndices> critical_canvases_with_indices_triangle_in_canvas(Canvas c);

    void dfs_code(int u, int idx, int& c, vector<int>& an, PrecoloredPathAndTriangleGraphCode& code) const;

    PrecoloredPathAndTriangleGraphCode compute_code_edge(int u, int v) const;

    PrecoloredPathAndTriangleGraphCode compute_code() const;

    int distance_between_path_and_triangle() const;

    bool test_no_l3_adjacent() const;

    //Tests that there is no list of size <= 4 adjacent to the triangle
    bool test_no_l4_adjacent_to_triangle() const;
    
    int count_l3_adjacent_pairs() const;

    bool test_criticality() const;

};



#endif