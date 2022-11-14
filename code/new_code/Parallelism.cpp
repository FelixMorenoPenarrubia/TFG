#include "Parallelism.hh"

std::mutex Parallelism::canvas_list_queue_mutex;
std::mutex Parallelism::recursive_reducibility_test_mutex;
std::mutex Parallelism::alon_tarsi_mutex;
std::mutex Parallelism::parallelism_finished_mutex;
std::vector<std::pair<int, std::thread>> Parallelism::thread_vector;
std::set<int> Parallelism::finished_set;

int Parallelism::curr_id = 0;

void Parallelism::clear_finished_threads() {
    Parallelism::parallelism_finished_mutex.lock();
    for (auto it = thread_vector.begin(); it != thread_vector.end(); ) {
        if (finished_set.find(it->first) == finished_set.end()) {
            it++;
        }
        else {
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
}

void Parallelism::spawn_thread_addcanvas(Canvas g, CanvasList& cl) {
    spawn_thread([&, g]() {
        CanvasSearch::add_canvas_real(g, cl);
    });
}

void Parallelism::spawn_thread_addcanvas_q(Canvas g, CanvasList& cl, std::queue<CanvasCode>& q) {
    spawn_thread([&, g]() {
        CanvasSearch::add_canvas_q_real(g, cl, q);
    });
}