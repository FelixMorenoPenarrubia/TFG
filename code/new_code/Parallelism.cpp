#include "Parallelism.hh"
#include<chrono>

std::mutex Parallelism::canvas_list_queue_mutex;
std::mutex Parallelism::recursive_reducibility_test_mutex;
std::mutex Parallelism::alon_tarsi_mutex;
std::mutex Parallelism::parallelism_finished_mutex;
std::mutex Parallelism::two_triangle_list_mutex;
std::mutex Parallelism::canvas_hash_list_mutex;
std::vector<std::pair<int, std::thread>> Parallelism::thread_vector;
std::set<int> Parallelism::finished_set;

int Parallelism::curr_id = 0;
int Parallelism::active_cnt = 0;

void Parallelism::clear_finished_threads() {
    Parallelism::parallelism_finished_mutex.lock();
    for (auto it = thread_vector.begin(); it != thread_vector.end(); ) {
        if (finished_set.find(it->first) == finished_set.end()) {
            it++;
        }
        else {
            finished_set.erase(finished_set.find(it->first));
            it->second.join();
            it = thread_vector.erase(it);
        }
    }
    Parallelism::parallelism_finished_mutex.unlock();
}

void Parallelism::wait_for_one_thread() {
    thread_vector.back().second.join();
    thread_vector.pop_back();
}


void Parallelism::wait_for_threads() {
    while (!thread_vector.empty()) {
        thread_vector.back().second.join();
        thread_vector.pop_back();
    }
}

void Parallelism::spawn_thread(std::function<void()> f) {
    #ifdef PARALLEL
    if (thread_vector.size() == MAX_THREADS) {
        clear_finished_threads();
        if (thread_vector.size() == MAX_THREADS) {
            wait_for_one_thread();
        }
    }
    int cid = curr_id; 
    thread_vector.emplace_back(cid, [=] {
        f();
        Parallelism::parallelism_finished_mutex.lock();
        Parallelism::finished_set.insert(cid);
        Parallelism::parallelism_finished_mutex.unlock();
    });
    curr_id++;
    #endif
}

/*
void Parallelism::spawn_thread_free_count(std::function<void()> f) {
    #ifdef PARALLEL
    std::thread t = std::thread([f] {
        Parallelism::wait_for_threads_free();
        Parallelism::parallelism_finished_mutex.lock();
        Parallelism::active_cnt++;
        Parallelism::parallelism_finished_mutex.unlock();
        f();
        Parallelism::parallelism_finished_mutex.lock();
        Parallelism::active_cnt--;
        Parallelism::parallelism_finished_mutex.unlock();
    });
    t.detach();
    #endif
}
*/



void Parallelism::spawn_thread_addcanvas(Canvas g, CanvasList& cl) {
    spawn_thread([&, g]() {
        CanvasSearch::add_canvas_real(g, cl);
    });
}

void Parallelism::spawn_thread_addcanvas_q(Canvas g, std::queue<CanvasCode>& q) {
    spawn_thread([&, g]() {
        CanvasSearch::add_canvas_q_real(g, q);
    });
}

void Parallelism::spawn_thread_addcanvas_st(Canvas g, std::vector<CanvasCode>& st) {
    spawn_thread([&, g]() {
        CanvasSearch::add_canvas_st_real(g, st);
    });
}

void Parallelism::spawn_thread_addcanvas_q_and_cl(Canvas g, CanvasList& cl, std::queue<CanvasCode>& q) {
    spawn_thread([&, g]() {
        CanvasSearch::add_canvas_q_and_cl_real(g, cl, q);
    });
}

/*
void Parallelism::wait_for_threads_free() {
    using namespace std::chrono_literals;
    while (true) {
        Parallelism::parallelism_finished_mutex.lock();
        bool good = Parallelism::active_cnt < Parallelism::MAX_THREADS;
        Parallelism::parallelism_finished_mutex.unlock();
        if (good) break;
        std::this_thread::sleep_for(50ms);
    }
}
*/