#ifndef CANVAS_HH_
#define CANVAS_HH_

#include "PrecoloredGraph.hh"

#include<iostream>
#include<vector>
#include<map>
#include<algorithm>
#include<string>



struct CanvasCode {
    std::vector<int> code;

    CanvasCode();

    CanvasCode(const std::string& s);

    void push(int x);
    
    void push_f();

    void push_b();

    int size() const;

    std::string to_string() const;

    int operator[](int i) const;

    bool operator<(const CanvasCode& b) const;


};

struct Canvas : public PrecoloredGraph {


    int l;


    // FUNCTIONS

    void init_inherited_values();

    Canvas();

    Canvas(std::vector<std::vector<int>> _al, int _l);

    Canvas(const CanvasCode& code);

    void write(std::ostream& os) const;

    static Canvas empty_cycle(int l);

    Canvas add_tripod(int s, int j, long long bm) const;

    static Canvas fuse_chord(Canvas g1, Canvas g2, int j1, int j2, bool same_orientation);

    void dfs_code(int u, int idx, int& c, vector<int>& an, CanvasCode& code) const;

    CanvasCode compute_code_edge(int u, int v) const;

    CanvasCode compute_code() const;

    bool test_criticality() const;

};

#endif