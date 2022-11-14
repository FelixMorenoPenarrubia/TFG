#ifndef CANVASSEARCH_HH_
#define CANVASSEARCH_HH_

#include "Canvas.hh"

#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<queue>
#include<algorithm>





using CanvasList = std::set<CanvasCode>;

struct CanvasSearch {
    
    std::vector<CanvasList> critical_chordless;
    std::vector<CanvasList> critical_with_chords;



    CanvasSearch();

    static void add_canvas(const Canvas& g, CanvasList& cl);

    static void add_canvas_q(const Canvas& g, CanvasList& cl, std::queue<CanvasCode>& q);

    static void add_canvas_real(const Canvas& g, CanvasList& cl);

    static void add_canvas_q_real(const Canvas& g, CanvasList& cl, std::queue<CanvasCode>& q);

    static bool test_canvas(const Canvas& g);

    void add_smaller_chords(int l, const std::vector<CanvasList>& prev, CanvasList& curr) const;

    void add_smaller_tripods(int l, const std::vector<CanvasList>& prev, CanvasList& curr) const;

    void add_same_size_tripods(int l, CanvasList& curr) const;

    void add_same_size_tripods_parallel(int l, CanvasList& curr) const;

    


    CanvasList generate_chordless(int l) const;
    
    CanvasList generate_with_chords(int l) const;

    vector<Canvas> get_chordless(int l);

    vector<Canvas> get_with_chords(int l);

};

#endif