#include "CanvasSearch.hh"
#include "debug_utility.hh"
#include <queue>
#include<iostream>

using ll = long long; //for bitmasks

CanvasSearch::CanvasSearch() {
    critical_chordless = vector<CanvasList>(3);
    critical_with_chords = vector<CanvasList>(3);
}

bool CanvasSearch::test_canvas(const Canvas& g) const {
    
    bool res = g.test_criticality(); 
    
    return res;
}

bool CanvasSearch::add_canvas(const Canvas& g, CanvasList& cl) const {

    CanvasCode cd = g.compute_code();
    if (cl.find(cd) == cl.end()) {

        

        if (test_canvas(g)) {
            
            if (g.l == 12 && g.n > 20) {
                
                debug_msg("Added canvas");
                debug_var(g.n);
                debug_var(g.al);
                debug_var(g.compute_code().to_string());
                
            }

            cl[cd] = g;
            return true;
        }
    }
    return false;
}

void CanvasSearch::add_smaller_chords(int l, const vector<CanvasList>& prev, CanvasList& curr) const {
    for(int a=3; a < l; ++a) {
        int b = l-a+2;
        for(auto p1 : prev[a]) {
            for(auto p2 : prev[b]) {
                Canvas g1 = p1.second;
                Canvas g2 = p2.second;
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
			Canvas g = p.second;
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
    std::queue<Canvas> q;
    for (auto p : curr) {
        q.push(p.second);
    }
    while(!q.empty()) {
		Canvas g = q.front();
		q.pop();
		for(int j=0; j < l; ++j) { //for each outer vertex of the canvas
			Canvas ng = g.add_tripod(1, j, 1);
			if(add_canvas(ng, curr)) q.push(ng);
		}
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
        ans.push_back(p.second);
    }
    return ans;
}

vector<Canvas> CanvasSearch::get_with_chords(int l) {
    for (int i=(int)critical_with_chords.size(); i <= l; ++i) {
        critical_with_chords.push_back(generate_with_chords(i));
    }
    vector<Canvas> ans;
    for (auto p : critical_with_chords[l]) {
        ans.push_back(p.second);
    }
    return ans;
}