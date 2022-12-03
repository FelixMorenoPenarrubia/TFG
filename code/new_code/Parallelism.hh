#ifndef PARALLELISM_HH_
#define PARALLELISM_HH_

#include<queue>
#include<thread>
#include<mutex>
#include<vector>
#include<functional>
#include<set>
#include<utility>

#include "Canvas.hh"
#include "CanvasSearch.hh"

//TODO: Come up with a less horrible way of implementing parallelism

struct Parallelism {

    static const int MAX_THREADS = 16;

    static int curr_id;
    static int active_cnt;
    static std::vector<std::pair<int, std::thread>> thread_vector;
    static std::set<int> finished_set;


    static std::mutex canvas_list_queue_mutex;
    static std::mutex recursive_reducibility_test_mutex;
    static std::mutex alon_tarsi_mutex;
    static std::mutex parallelism_finished_mutex;
    static std::mutex two_triangle_list_mutex;
    static std::mutex canvas_hash_list_mutex;

    static void clear_finished_threads();

    static void wait_for_one_thread();

    static void wait_for_threads();

    //TODO: true generic thread spawning function - ugh, I need to do this better
    static void spawn_thread(std::function<void()> f);

    static void spawn_thread_addcanvas(Canvas g, CanvasList& cl);

    static void spawn_thread_addcanvas_q(Canvas g, std::queue<CanvasCode>& q);

    static void spawn_thread_addcanvas_st(Canvas g, std::vector<CanvasCode>& st);

    static void spawn_thread_addcanvas_q_and_cl(Canvas g, CanvasList& cl, std::queue<CanvasCode>& q);

};


#endif