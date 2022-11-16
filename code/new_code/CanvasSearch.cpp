#include "CanvasSearch.hh"
#include "debug_utility.hh"
#include "Parallelism.hh"
#include <queue>
#include<iostream>
#include<thread>

using ll = long long; //for bitmasks

CanvasSearch::CanvasSearch() {
    critical_chordless = vector<CanvasList>(3);
    critical_with_chords = vector<CanvasList>(3);
}

bool CanvasSearch::test_canvas(const Canvas& g) {
    
    bool res = g.test_criticality(); 
    
    return res;
}

void CanvasSearch::add_canvas_real(const Canvas& g, CanvasList& cl) {
    CanvasCode cd = g.compute_code();
    #ifdef PARALLEL
    Parallelism::canvas_list_queue_mutex.lock();
    #endif
    bool nf = cl.find(cd) == cl.end();
    #ifdef PARALLEL
    Parallelism::canvas_list_queue_mutex.unlock();
    #endif
    if (nf) {
        if (test_canvas(g)) {
            #ifdef PARALLEL
            Parallelism::canvas_list_queue_mutex.lock();
            #endif
            cl.insert(cd);
            #ifdef PARALLEL
            Parallelism::canvas_list_queue_mutex.unlock();
            #endif
        }
    }
}

void CanvasSearch::add_canvas_q_real(const Canvas& g, CanvasList& cl, std::queue<CanvasCode>& q) {
     CanvasCode cd = g.compute_code();
    #ifdef PARALLEL
    Parallelism::canvas_list_queue_mutex.lock();
    #endif
    bool nf = cl.find(cd) == cl.end();
    #ifdef PARALLEL
    Parallelism::canvas_list_queue_mutex.unlock();
    #endif
    if (nf) {
        if (test_canvas(g)) {
            #ifdef PARALLEL
            Parallelism::canvas_list_queue_mutex.lock();
            #endif
            if (cl.find(cd) == cl.end()) {
                q.push(cd);
                cl.insert(cd);
            }
            #ifdef PARALLEL
            Parallelism::canvas_list_queue_mutex.unlock();
            #endif
        }
    }
}

void CanvasSearch::add_canvas(const Canvas& g, CanvasList& cl) {
    #ifdef PARALLEL
    //Parallelism::thread_queue.push(std::thread(add_canvas_real, g, std::ref(cl)));
    Parallelism::spawn_thread_addcanvas(g, cl);
    #else
    add_canvas_real(g, cl);
    #endif
}

void CanvasSearch::add_canvas_q(const Canvas& g, CanvasList& cl, std::queue<CanvasCode>& q) {
    #ifdef PARALLEL
    //Parallelism::thread_queue.push(std::thread(add_canvas_q_real, g, std::ref(cl), std::ref(q)));
    Parallelism::spawn_thread_addcanvas_q(g, cl, q);
    #else
    add_canvas_q_real(g, cl, q);
    #endif
}

void CanvasSearch::add_smaller_chords(int l, const vector<CanvasList>& prev, CanvasList& curr) const {
    for(int a=3; a < l; ++a) {
        int b = l-a+2;
        for(auto p1 : prev[a]) {
            for(auto p2 : prev[b]) {
                Canvas g1 = Canvas(p1);
                Canvas g2 = Canvas(p2);
                for(int j1=0; j1 < a; ++j1) {
                    for(int j2=0; j2 < b; ++j2) {
                        Canvas ng1 = Canvas::fuse_chord(g1, g2, j1, j2, false);
                        Canvas ng2 = Canvas::fuse_chord(g1, g2, j1, j2, true);
                        add_canvas(ng1, curr);
                        add_canvas(ng2, curr);
                    }
                }
            }
        }
    }
}

void CanvasSearch::add_smaller_tripods(int l, const vector<CanvasList>& prev, CanvasList& curr) const {
    for(int k=3; k < l; ++k) {  //for each canvas size up to l-1
		ll ts = l-k+1;
		for(auto p : prev[k]) { //for each canvas of that size
			Canvas g = Canvas(p);
			for(int j=0; j < k; ++j) { //for each outer vertex of the canvas
				for(ll bm=1; bm < (1LL<<ts); ++bm) { //for each disposition of the chords of the tripod (bitmask)
					Canvas ng = g.add_tripod(ts, j, bm);
					add_canvas(ng, curr);
				}
			}
		}
	}
}



void CanvasSearch::add_same_size_tripods(int l, CanvasList& curr) const {
    #ifdef PARALLEL
    add_same_size_tripods_parallel(l, curr);
    return;
    #endif

    std::queue<CanvasCode> q;
    for (auto p : curr) {
        q.push(p);
    }
    while(!q.empty()) {
		Canvas g = Canvas(q.front());
		q.pop();
		for(int j=0; j < l; ++j) { //for each outer vertex of the canvas
			Canvas ng = g.add_tripod(1, j, 1);
			add_canvas_q(ng, curr, q);
		}
	}
}



void CanvasSearch::add_same_size_tripods_parallel(int l, CanvasList& curr) const {
   
    Parallelism::wait_for_threads();

    std::queue<CanvasCode> q;
    for (auto p : curr) {
        q.push(p);
    }

    while(true) {

        Parallelism::canvas_list_queue_mutex.lock();
		Canvas g = Canvas(q.front());
		q.pop();
        Parallelism::canvas_list_queue_mutex.unlock();
		for(int j=0; j < l; ++j) { //for each outer vertex of the canvas
			Canvas ng = g.add_tripod(1, j, 1);
			add_canvas_q(ng, curr, q);
		}
        Parallelism::canvas_list_queue_mutex.lock();
        bool empt = q.empty();
        Parallelism::canvas_list_queue_mutex.unlock();
        if (empt) {
            Parallelism::wait_for_threads();
        }
        Parallelism::canvas_list_queue_mutex.lock();
        empt = q.empty();
        Parallelism::canvas_list_queue_mutex.unlock();
        if (empt) break;
	}
}

CanvasList CanvasSearch::generate_chordless(int l) const {
    CanvasList ans;
    add_canvas(Canvas::empty_cycle(l), ans);
    add_smaller_tripods(l, critical_chordless, ans);
    add_same_size_tripods(l, ans);
    return ans;
}

CanvasList CanvasSearch::generate_with_chords(int l) const {
    CanvasList ans;
    add_canvas(Canvas::empty_cycle(l), ans);
    add_smaller_chords(l, critical_with_chords, ans);
    add_smaller_tripods(l, critical_with_chords, ans);
    add_same_size_tripods(l, ans);
    return ans;
}

vector<Canvas> CanvasSearch::get_chordless(int l) {
    for (int i=(int)critical_chordless.size(); i <= l; ++i) {
        critical_chordless.push_back(generate_chordless(i));
    }
    vector<Canvas> ans;
    for (auto p : critical_chordless[l]) {
        ans.push_back(Canvas(p));
    }
    return ans;
}

vector<Canvas> CanvasSearch::get_with_chords(int l) {
    for (int i=(int)critical_with_chords.size(); i <= l; ++i) {
        critical_with_chords.push_back(generate_with_chords(i));
    }
    vector<Canvas> ans;
    for (auto p : critical_with_chords[l]) {
        ans.push_back(Canvas(p));
    }
    return ans;
}

std::set<CanvasCode>& CanvasSearch::get_chordless_code(int l) {
    for (int i=(int)critical_chordless.size(); i <= l; ++i) {
        critical_chordless.push_back(generate_chordless(i));
    }
    return critical_chordless[l];
}