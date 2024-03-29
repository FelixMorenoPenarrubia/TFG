#ifndef CANVASSEARCH_HH_
#define CANVASSEARCH_HH_

#include "Canvas.hh"

#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<queue>
#include<algorithm>
#include<unordered_set>
#include<thread>

#include "parallel_hashmap/phmap.h"







using CanvasList = std::set<CanvasCode>;
using CanvasHashList = phmap::parallel_flat_hash_set<cchash>;


struct CanvasSearch {
    
    std::vector<CanvasList> critical_chordless;
    std::vector<CanvasList> critical_with_chords;

    static bool DFS_MODE;
    static bool HALFMEMORY_MODE;
    static CanvasHashList ch;

    #ifdef PARALLEL
    static std::vector<std::thread> root_threads;
    #endif



    CanvasSearch();

    static void add_canvas(const Canvas& g, CanvasList& cl);

    static void add_canvas_dfs(const Canvas& g);

    static void add_canvas_dfs_serial(const Canvas& g);

    static void add_canvas_dfs_parallel(const Canvas& g);

    static void add_canvas_dfs_parallel_2(const Canvas& g);

    static void process_canvas_dfs_parallel(const Canvas& g);

    static void add_canvas_st_real(const Canvas& g, std::vector<CanvasCode>& st);

    static void add_canvas_q(const Canvas& g, std::queue<CanvasCode>& q);

    static void add_canvas_q_and_cl(const Canvas& g, CanvasList& cl, std::queue<CanvasCode>& q);

    static void add_canvas_real(const Canvas& g, CanvasList& cl);

    static void add_canvas_q_real(const Canvas& g, std::queue<CanvasCode>& q);

    static void add_canvas_q_and_cl_real(const Canvas& g, CanvasList& cl, std::queue<CanvasCode>& q);

    static bool test_canvas(const Canvas& g);

    void add_smaller_chords(int l, const std::vector<CanvasList>& prev, CanvasList& curr) const;

    void add_smaller_tripods(int l, const std::vector<CanvasList>& prev, CanvasList& curr) const;

    void add_same_size_tripods(int l, CanvasList& curr) const;

    void add_same_size_tripods_halfmemory(int l, CanvasList& curr) const;

    void add_same_size_tripods_parallel(int l, CanvasList& curr) const;

    void add_same_size_tripods_parallel_halfmemory(int l, CanvasList& curr) const;

    


    CanvasList generate_chordless(int l) const;
    
    CanvasList generate_with_chords(int l) const;

    vector<Canvas> get_chordless(int l);

    vector<Canvas> get_with_chords(int l);

    std::set<CanvasCode>& get_chordless_code(int l);

    void print_chordless_lessmemory(int l);

    void read_previous_and_print_lessmemory();

};

#endif