#include "TwoPrecoloredPathsGraphSearch.hh"
#include <queue>
#include<iostream>
#include<thread>
#include<unistd.h>


using ll = long long; //for bitmasks

TwoPrecoloredPathsGraphSearch::TwoPrecoloredPathsGraphSearch() {
    critical = vector<std::set<TwoPrecoloredPathsGraphCode>>(1);
}

bool TwoPrecoloredPathsGraphSearch::test_graph(const TwoPrecoloredPathsGraph& g, int d) {
    
    bool res = (g.distance_between_paths() == d && g.test_criticality()); 
    return res;
}

void TwoPrecoloredPathsGraphSearch::add_graph(const TwoPrecoloredPathsGraph& g, std::set<TwoPrecoloredPathsGraphCode>& gl, int d) {
    TwoPrecoloredPathsGraphCode cd = g.compute_code();
    bool nf = gl.find(cd) == gl.end();



    if (nf) {
        if (test_graph(g, d)) {
            gl.insert(cd);
        }
    }
}

void TwoPrecoloredPathsGraphSearch::add_same_side(int d, std::set<TwoPrecoloredPathsGraphCode>& gl) {
    std::set<PrecoloredPathGraphCode> v1 = ppsearch.get_all_codes(d+3);
    std::set<PrecoloredPathGraphCode> v2 = ppsearch.get_all_codes(d+1);
    for (PrecoloredPathGraphCode c1 : v1) {
        PrecoloredPathGraph g1 = PrecoloredPathGraph(c1);
        for (PrecoloredPathGraphCode c2 : v2) {
            PrecoloredPathGraph g2 = PrecoloredPathGraph(c2);
            add_graph(TwoPrecoloredPathsGraph::fuse_precoloredpaths_sameside(g1, g2), gl, d);
            add_graph(TwoPrecoloredPathsGraph::fuse_precoloredpaths_sameside(g1, g2.reverse()), gl, d);
        }
    }
}

void TwoPrecoloredPathsGraphSearch::add_opposite_side(int d, std::set<TwoPrecoloredPathsGraphCode>& gl) {
    std::set<PrecoloredPathGraphCode> v = ppsearch.get_all_codes(d+2);
    for (PrecoloredPathGraphCode c1 : v) {
        PrecoloredPathGraph g1 = PrecoloredPathGraph(c1);
        for (PrecoloredPathGraphCode c2 : v) {
            PrecoloredPathGraph g2 = PrecoloredPathGraph(c2);
            add_graph(TwoPrecoloredPathsGraph::fuse_precoloredpaths_oppositeside(g1, g2), gl, d);
            add_graph(TwoPrecoloredPathsGraph::fuse_precoloredpaths_oppositeside(g1, g2.reverse()), gl, d);
        }
    }
}

std::set<TwoPrecoloredPathsGraphCode> TwoPrecoloredPathsGraphSearch::generate(int d) {
    std::set<TwoPrecoloredPathsGraphCode> ans;
    add_same_side(d, ans);
    add_opposite_side(d, ans); 
    return ans;
}




vector<TwoPrecoloredPathsGraph> TwoPrecoloredPathsGraphSearch::get(int d) {
    for (int i=(int)critical.size(); i <= d; ++i) {
        critical.push_back(generate(d));
    }
    vector<TwoPrecoloredPathsGraph> ans;
    for (auto p : critical[d]) {
        
        ans.push_back(TwoPrecoloredPathsGraph(p));
      
    }
    return ans;
}

