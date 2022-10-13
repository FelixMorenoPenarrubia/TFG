#include "Graph.h"
#include "AlonTarsi.h"
#include "CanvasToTriangles.h"
#include "debug_utility.h"
#include<iostream>
#include<vector>
#include<map>


using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::map;
using namespace precolored;

bool test_graph (const Graph& g) {
    
   return !recursive_alon_tarsi(g);
}

vector<pair<Graph, vector<vector<int> > > > read_canvas_and_generate_triangle_graphs() {
    int n, m, l;
    cin >> n >> m >> l;
    vector<vector<int> > al (n);
    vector<std::map<int, int> > ral(n);
    vector<vector<int> > am (n, vector<int> (n));
    for (int i=0; i < 2*m; ++i) {
        int u, v;
        cin >> u >> v;
        ral[u][v] = al[u].size();
        al[u].push_back(v);
        am[u][v] = 1;
    }

    int path_length = (l-6)/2;
    vector<pair<Graph, vector<vector<int>> >> ans;

    for (int s=0; s < l; ++s) {
        int nn = n-l+(path_length-1);
        vector<int> list_sizes(nn, 5);
        vector<vector<int> > nal(nn);
        vector<vector<int> > nam(nn, vector<int> (nn));
        vector<int> morph(n, -1);
        for (int i=1; i < path_length; ++i) {
            morph[(s+i)%l] = i-1;
            morph[(s+2*path_length+3-i)%l] = i-1;
        }
        for (int i=l; i < n; ++i) {
            morph[i] = i-l+(path_length-1);
        }
        

        for (int i=1; i < path_length; ++i) {
            int st = ral[(s+i)%l][(s+i+1)%l];
            int sz = (int)al[(s+i)%l].size();
            for (int j = (i == path_length-1 ? 1 : 0); j < (i == 1 ? sz-1 : sz); ++j) {
                int x = morph[(s+i)%l];
                int y = morph[al[(s+i)%l][(j+st)%sz]];
                if (!nam[x][y]) {
                    nal[x].push_back(y);
                    nam[x][y] = 1;
                }
            }
            int st2 = ral[(s+2*path_length+3-i)%l][(s+2*path_length+3-i+1)%l];
            int sz2 = (int)al[(s+2*path_length+3-i)%l].size();
            for (int j=1; j+1 < sz2; ++j) {
                int x = morph[(s+i)%l];
                int y = morph[al[(s+2*path_length+3-i)%l][(j+st2)%sz2]];
                if (!nam[x][y]) {
                    nal[x].push_back(y);
                    nam[x][y] = 1;
                }
            }
        }

        for (int u = 0; u < n; ++u) {
            if (morph[u] == -1) continue;
            if (u >= l) {
                for (int v : al[u]) {
                    if (morph[v] == -1) continue;
                    else if (v < l && v != (morph[v]+1+s)%l && (ral[u].find((morph[v]+1+s)%l) != ral[u].end())) continue;
                    nal[morph[u]].push_back(morph[v]);
                    nam[morph[u]][morph[v]] = 1;
                }
                if (am[u][s] || am[u][(s+2*path_length+3)%l]) list_sizes[morph[u]]--;
                if (am[u][(s+path_length)%l] || am[u][(s+path_length+3)%l]) list_sizes[morph[u]]--;
                if (am[u][(s+1*path_length+1)%l]) list_sizes[morph[u]]--;
                if (am[u][(s+1*path_length+2)%l]) list_sizes[morph[u]]--;
                if (am[u][(s+2*path_length+4)%l]) list_sizes[morph[u]]--;
                if (am[u][(s+2*path_length+5)%l]) list_sizes[morph[u]]--;
            }
            else {
                if (u == (s+1)%l || u == (s+path_length-1)%l) {
                    list_sizes[morph[u]]--; //assuming no chords
                    if (path_length == 2) list_sizes[morph[u]]--;
                }
            }
        }
        ans.emplace_back(Graph(list_sizes, nal), al);
        
        //ans.back().first.write(cerr);
    }

    return ans;
}

void print_result(pair<Graph, vector<vector<int>> > p) {
    cout << "---" << endl;
    cout << "Graph with list sizes: " << endl;
    p.first.write(cout);
    cout << "Original canvas: " << endl;
    int n = p.second.size();
    cout << n << endl;
    for (int i=0; i < n; ++i) {
        for (int j : p.second[i]) {
            cout << i << " " << j << endl;
        }
    }
    cout << "---" << endl;
}

void process_canvas(map<Code, pair<Graph, vector<vector<int>> >>& critical_set) {
    vector<pair<Graph, vector<vector<int>> >> candidate_list = read_canvas_and_generate_triangle_graphs();
    
    //debug(candidate_list.size());

    map<Code, int> candidate_set;
    for (int i=0; i < (int) candidate_list.size(); ++i) {
        Code gc = candidate_list[i].first.compute_code();
        if (candidate_set.find(gc) == candidate_set.end() && critical_set.find(gc) == critical_set.end()) {
            candidate_set[gc] = i; 
        }
    }


    for (auto p : candidate_set) {
        const Graph& g = candidate_list[p.second].first;

        if (test_graph(g)) {
            critical_set[p.first] = candidate_list[p.second];
        }
    }

    

   
}

void read_canvases_and_print_critical_triangles() {

    map<Code, pair<Graph, vector<vector<int>> >> critical_set;
    int T;
    cin >> T;
    for (int t = 0; t < T; ++t) {
        process_canvas(critical_set);
    }

    cout << critical_set.size() << endl;

    for (auto x : critical_set) {
        print_result(x.second);
    }
}

