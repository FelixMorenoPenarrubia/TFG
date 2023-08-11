#include "RestrictedPrecoloredPathGraphSearch.hh"
#include <queue>
#include<iostream>
#include<thread>
#include<unistd.h>


using ll = long long; //for bitmasks

RestrictedPrecoloredPathGraphSearch::RestrictedPrecoloredPathGraphSearch() {
    critical_all = vector<GraphList>(4);
    critical_all[2].insert(PrecoloredPathGraph({{1}, {0}}, 2, {1, 1}).compute_code());
    critical_all[3].insert(PrecoloredPathGraph({{1}, {2, 0}, {1}}, 3, {1, 1, 1}).compute_code());
    critical_all[3].insert(PrecoloredPathGraph({{1, 2}, {2, 0}, {0, 1}}, 3, {1, 1, 1}).compute_code());
    critical_all[3].insert(PrecoloredPathGraph({{1, 3}, {2, 3, 0}, {3, 1}, {0, 1, 2}}, 3, {1, 1, 1, 3}).compute_code());
    critical_all[3].insert(PrecoloredPathGraph({{1, 4}, {2, 3, 4, 0}, {3, 1}, {4, 1, 2}, {0, 1, 3}}, 3, {1, 1, 1, 3, 3}).compute_code());
    //Not sure if necessary
   // critical_all[3].insert(PrecoloredPathGraph({{1, 5, 4}, {2, 5, 0}, {3, 5, 1}, {4, 5, 2}, {0, 5, 3}, {0, 1, 2, 3, 4}}, 3, {1, 1, 1, 3, 3, 5}).compute_code());
    critical_biconnected = vector<GraphList>(4);
    critical_biconnected[2].insert(PrecoloredPathGraph({{1}, {0}}, 2, {1, 1}).compute_code());
    critical_biconnected[3].insert(PrecoloredPathGraph({{1, 2}, {2, 0}, {0, 1}}, 3, {1, 1, 1}).compute_code());
    critical_biconnected[3].insert(PrecoloredPathGraph({{1, 3}, {2, 3, 0}, {3, 1}, {0, 1, 2}}, 3, {1, 1, 1, 3}).compute_code());
    critical_biconnected[3].insert(PrecoloredPathGraph({{1, 4}, {2, 3, 4, 0}, {3, 1}, {4, 1, 2}, {0, 1, 3}}, 3, {1, 1, 1, 3, 3}).compute_code());
    //Not sure if necessary
    //critical_biconnected[3].insert(PrecoloredPathGraph({{1, 5, 4}, {2, 5, 0}, {3, 5, 1}, {4, 5, 2}, {0, 5, 3}, {0, 1, 2, 3, 4}}, 3, {1, 1, 1, 3, 3, 5}).compute_code());
}

bool RestrictedPrecoloredPathGraphSearch::allowed_wedge(const PrecoloredPathGraph& g) const {
    if (g.n == 0) return true;
    /*
    std::set<PrecoloredPathGraphCode> allowed;
    allowed.insert(PrecoloredPathGraph({{1}, {0}}, 2, {1, 1}).compute_code());
    allowed.insert(PrecoloredPathGraph({{1, 2}, {2, 0}, {0, 1}}, 3, {1, 1, 1}).compute_code());
    allowed.insert(PrecoloredPathGraph({{1, 3}, {2, 3, 0}, {3, 1}, {0, 1, 2}}, 3, {1, 1, 1, 3}).compute_code());
    allowed.insert(PrecoloredPathGraph({{1, 4}, {2, 3, 4, 0}, {3, 1}, {4, 1, 2}, {0, 1, 3}}, 3, {1, 1, 1, 3, 3}).compute_code());
    return allowed.find(g.compute_code()) != allowed.end();
    */
   return critical_biconnected[g.l].find(g.compute_code()) != critical_biconnected[g.l].end();
}

bool RestrictedPrecoloredPathGraphSearch::test_graph(const PrecoloredPathGraph& g_un) const {

    PrecoloredPathGraph g = PrecoloredPathGraph(g_un.compute_code());
     
    if (g.has_large_fans_or_bellows()) return false;
    //if (g.has_bellows_in_endpoints()) return false;

    if (!allowed_wedge(g.largest_cuts(1).first) || !allowed_wedge(g.largest_cuts(g.l-2).second)) return false;

    

    for (int i=1; i+2 < g.l; ++i) {
        auto p = g.largest_cuts(i);
        if (p.second.n > 0) {
            for (int j=1; j+i+(i==1) < g.l; ++j) {
                PrecoloredPathGraph w = p.second.largest_cuts(j).first;
                if (!allowed_wedge(w)) return false;
            }
        }
    }
    return g.test_criticality();
}

void register_graph(const PrecoloredPathGraph& g) {

    //return;

   /* if (g.l >= 5) {
        std::cerr << "reg " << g.n << " " << g.m << " " << g.compute_code().to_string() << std::endl;
    }*/
    static int max_mn = 0;
    if (g.n + g.m > max_mn) {
        PrecoloredPathGraph(g.compute_code()).write(std::cerr);
        max_mn = g.n+g.m;
    }
}

void RestrictedPrecoloredPathGraphSearch::add_graph(const PrecoloredPathGraph& g, GraphList& cl) const {
    PrecoloredPathGraphCode cd = g.compute_code();
    bool nf = cl.find(cd) == cl.end();
    if (nf) {
        if (test_graph(g)) {
            register_graph(g);
            cl.insert(cd);
        }
    }
}

void RestrictedPrecoloredPathGraphSearch::add_graph_q_and_cl(const PrecoloredPathGraph& g, GraphList& cl, std::queue<PrecoloredPathGraphCode>& q) const {
    
    PrecoloredPathGraphCode cd = g.compute_code();
    bool nf = cl.find(cd) == cl.end();
    if (nf) {
        if (test_graph(g)) {
            register_graph(g);
            q.push(cd);
            cl.insert(cd);
        }
    }
    
}

void RestrictedPrecoloredPathGraphSearch::add_smaller_crossing_chords(int l, const vector<GraphList>& prev, GraphList& curr) const {
    for(int a=4; a < l; ++a) { //for all graphs with l from 4 to l-1 - the case with (3, l) is treated in the same size function
        int b = l-a+3;
        for(auto p1 : prev[a]) {
            for(auto p2 : prev[b]) {
                PrecoloredPathGraph g1 = PrecoloredPathGraph(p1);
                PrecoloredPathGraph g2 = PrecoloredPathGraph(p2);
                //TODO: check if it is necessary to use both orders
                add_graph(PrecoloredPathGraph::fuse_chord(g1, g2, 3), curr);
                add_graph(PrecoloredPathGraph::fuse_chord(g1, g2, 4), curr);
                add_graph(PrecoloredPathGraph::fuse_chord(g1, g2, 5), curr);
                add_graph(PrecoloredPathGraph::fuse_chord(g2, g1, 3), curr);
                add_graph(PrecoloredPathGraph::fuse_chord(g2, g1, 4), curr);
                add_graph(PrecoloredPathGraph::fuse_chord(g2, g1, 5), curr);
            }
        }
    }
}

void RestrictedPrecoloredPathGraphSearch::add_smaller_articulation_points(int l, const vector<GraphList>& prev, GraphList& curr) const {
    for(int a=2; a < l; ++a) { //for all graphs with l from 2 to l-1
        int b = l-a+1;
        for(auto p1 : prev[a]) {
            for(auto p2 : prev[b]) {
                PrecoloredPathGraph g1 = PrecoloredPathGraph(p1);
                PrecoloredPathGraph g2 = PrecoloredPathGraph(p2);
                add_graph(PrecoloredPathGraph::fuse_articulation_point(g1, g2), curr);
                add_graph(PrecoloredPathGraph::fuse_articulation_point(g2, g1), curr);
            }
        }
    }
}

void RestrictedPrecoloredPathGraphSearch::add_smaller_path_chords(int l, const vector<GraphList>& prev, GraphList& curr) const {
    for(int a=3; a < l; ++a) { //for all graphs with l from 3 to l-1
        int b = l-a;
        for(auto p : prev[a]) {
            PrecoloredPathGraph g = PrecoloredPathGraph(p);
            for(int j=0; j < a-1; ++j) {
                add_graph(g.extend_path_chord(b, j), curr);
            }
        }
    }
}




void RestrictedPrecoloredPathGraphSearch::add_smaller_tripods(int l, const vector<GraphList>& prev, GraphList& curr) const {
    for(int k=3; k < l; ++k) {  //for each path size up to l-1
		ll ts = l-k+1;
		for(auto p : prev[k]) { //for each graph of that size
			PrecoloredPathGraph g = PrecoloredPathGraph(p);
			for(int j=1; j < k-1; ++j) { //for each inner vertex of the path
				for(ll bm=1; bm < (1LL<<ts); ++bm) { //for each disposition of the chords of the tripod (bitmask)
					PrecoloredPathGraph ng = g.add_tripod(ts, j, bm);
					add_graph(ng, curr);
				}
			}
		}
	}
}

void RestrictedPrecoloredPathGraphSearch::add_same_size_tripods_and_chords(int l, GraphList& curr) const {
    std::queue<PrecoloredPathGraphCode> q;
    for (auto p : curr) {
        q.push(p);
    }
    while(!q.empty()) {
        //if (curr.size() > 100000) return;

		PrecoloredPathGraph g = PrecoloredPathGraph(q.front());
		q.pop();
		for(int j=1; j < l-1; ++j) { //for each inner vertex of the path, add size 3 tripod
			PrecoloredPathGraph ng = g.add_tripod(1, j, 1);
			add_graph_q_and_cl(ng, curr, q);
		}
        //add size 3 chords
        for (auto c : critical_biconnected[3]) {
            PrecoloredPathGraph g2 = PrecoloredPathGraph(c);
            add_graph_q_and_cl(PrecoloredPathGraph::fuse_chord(g, g2, 3), curr, q);
            add_graph_q_and_cl(PrecoloredPathGraph::fuse_chord(g2, g, 3), curr, q);
            add_graph_q_and_cl(PrecoloredPathGraph::fuse_chord(g, g2, 4), curr, q);
            add_graph_q_and_cl(PrecoloredPathGraph::fuse_chord(g2, g, 4), curr, q);
            add_graph_q_and_cl(PrecoloredPathGraph::fuse_chord(g, g2, 5), curr, q);
            add_graph_q_and_cl(PrecoloredPathGraph::fuse_chord(g2, g, 5), curr, q);
        }
	}
}


GraphList RestrictedPrecoloredPathGraphSearch::generate_biconnected(int l) const {
    GraphList ans;
    add_graph(PrecoloredPathGraph::empty_cycle(l, vector<int>(l, 1)), ans);
    add_smaller_crossing_chords(l, critical_biconnected, ans);
    add_smaller_path_chords(l, critical_biconnected, ans);
    add_smaller_tripods(l, critical_biconnected, ans);
    add_same_size_tripods_and_chords(l, ans);
    return ans;
}

GraphList RestrictedPrecoloredPathGraphSearch::generate_all(int l) const {
    GraphList ans = critical_biconnected[l];
    add_graph(PrecoloredPathGraph::empty_path(l), ans);
    add_smaller_articulation_points(l, critical_all, ans);
    return ans;
}


vector<PrecoloredPathGraph> RestrictedPrecoloredPathGraphSearch::get_all(int l) {
    for (int i=(int)critical_biconnected.size(); i <= l; ++i) {
        critical_biconnected.push_back(generate_biconnected(i));
    }
    for (int i=(int)critical_all.size(); i <= l; ++i) {
        critical_all.push_back(generate_all(i));
    }
    vector<PrecoloredPathGraph> ans;
    for (auto p : critical_all[l]) {
        ans.push_back(PrecoloredPathGraph(p));
    }
    return ans;
}

GraphList RestrictedPrecoloredPathGraphSearch::get_all_codes(int l) {
    for (int i=(int)critical_biconnected.size(); i <= l; ++i) {
        critical_biconnected.push_back(generate_biconnected(i));
    }
    for (int i=(int)critical_all.size(); i <= l; ++i) {
        critical_all.push_back(generate_all(i));
    }
    return critical_all[l];
}

