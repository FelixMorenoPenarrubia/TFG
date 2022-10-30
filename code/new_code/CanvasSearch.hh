#ifndef CANVASSEARCH_HH_
#define CANVASSEARCH_HH_

#include "Canvas.hh"

#include<iostream>
#include<vector>
#include<map>
#include<algorithm>


using std::vector;
using std::map;

using CanvasList = map<CanvasCode, Canvas>;

struct CanvasSearch {
    
    vector<CanvasList> critical_chordless;
    vector<CanvasList> critical_with_chords;

    CanvasSearch();

    bool add_canvas(const Canvas& g, CanvasList& cl) const;

    bool test_canvas(const Canvas& g) const;

    void add_smaller_chords(int l, const vector<CanvasList>& prev, CanvasList& curr) const;

    void add_smaller_tripods(int l, const vector<CanvasList>& prev, CanvasList& curr) const;

    void add_same_size_tripods(int l, CanvasList& curr) const;

    CanvasList generate_chordless(int l) const;
    
    CanvasList generate_with_chords(int l) const;

    vector<Canvas> get_chordless(int l);

    vector<Canvas> get_with_chords(int l);

};

#endif