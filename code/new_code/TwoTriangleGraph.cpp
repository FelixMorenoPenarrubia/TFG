#include "TwoTriangleGraph.hh"

#include<iostream>

using std::vector;
using std::endl;

TwoTriangleGraph::TwoTriangleGraph(const Canvas& g, int s) {
    original_canvas = g;

    path_length = (g.l-6)/2;
    
    n = g.n-(path_length+1);
    al = vector<vector<int> >(n);


    vector<int> morph(g.n);
    vector<int> corr(g.n, -1);

    for (int i=0; i < path_length+1; ++i) {
        morph[(s+i)%g.l] = i;
        morph[(s+2*path_length+3-i)%g.l] = i;
        corr[((s+2*path_length+3-i)%g.l)] = (s+i)%g.l;
    }
    morph[(s+path_length+1)%g.l] = path_length+1;
    morph[(s+path_length+2)%g.l] = path_length+2;
    morph[(s-2+g.l)%g.l] = path_length+3;
    morph[(s-1+g.l)%g.l] = path_length+4;
    for (int i=g.l; i < g.n; ++i) {
        morph[i] = i-g.l+path_length+5;
    }

    precolored = vector<int>(n);
    precolored[0] = 1;
    precolored[path_length] = 1;
    precolored[path_length+1] = 1;
    precolored[path_length+2] = 1;
    precolored[path_length+3] = 1;
    precolored[path_length+4] = 1;

    vector<vector<int>> am(g.n, vector<int>(g.n));

    for (int u = 0; u < g.n; ++u) {
        for (int v : g.al[u]) {
            am[u][v] = 1;
        }
    }

    for (int i = 0; i < path_length+1; ++i) {
        int u = (s+i)%g.l;
        int nu = (s+i+1)%g.l;
        int us = g.al[u].size();
        for (int j=0;  j < us; ++j) {
            int v = g.al[u][(j+g.ral[u].at(nu))%us];
            al[morph[u]].push_back(morph[v]); //TODO: Assuming no chords
        }
        int cu = (s+2*path_length+3-i)%g.l;
        int cnu = (cu+1)%g.l;
        int cus = g.al[cu].size();
        for (int j=1;  j+1 < cus; ++j) {
            int v = g.al[cu][(j+g.ral[cu].at(cnu))%cus];
            if (!am[u][v]) al[morph[u]].push_back(morph[v]); 
        }

    }

    for (int u=g.l; u < g.n; ++u) {
        for (int j = (int)g.al[u].size()-1; j > -1; --j) {
            int v = g.al[u][j];
            if (corr[v] != -1 && am[u][corr[v]]) continue;
            al[morph[u]].push_back(morph[v]);
        }
    }

    generate_ral_and_m();  
}

void TwoTriangleGraph::write(std::ostream& os) const {
    os << n << " " << m << " " << path_length << endl;
    for (int i=0; i < n; ++i) {
        if (precolored[i]) os << i << " ";
    }
    os << endl;
    for (int u = 0; u < n; ++u) {
        for (int v : al[u]) {
            os << u << " " << v << endl;
        }
    }
}

vector<TwoTriangleGraph> TwoTriangleGraph::generate_from_canvas(const Canvas& g) {
    vector<TwoTriangleGraph> ans;
    for (int i=0; i < g.l; ++i) {
        ans.push_back(TwoTriangleGraph(g, i));
    }
    return ans;
}