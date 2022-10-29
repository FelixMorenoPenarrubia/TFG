#include "Graph.h"
#include "AlonTarsi.h"
#include "CanvasToTriangles.h"
#include "debug_utility.h"
#include<iostream>
#include<vector>
#include<map>
#include<fstream>


using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::map;

TwoTriangleGraph::TwoTriangleGraph() {

}

TwoTriangleGraph::TwoTriangleGraph(Graph _g, int _pl, vector<vector<int>> _oc, vector<vector<int>> _ett) {
    g = _g;
    path_length = _pl;
    original_canvas = _oc;
    edges_to_triangles = _ett;
}

void TwoTriangleGraph::write_verbose(std::ostream& os) {
    os << "---" << endl;
    os << "Graph with list sizes: " << endl;
    g.write(cout);
    //cout << "Code: " << endl;
    //cout << p.first.compute_code().to_string() << endl;
    os << "Edges to the two triangles: " << endl;
    for (int i=0; i < g.n; ++i) {
        for (int j : edges_to_triangles[i]) {
            os << i << " T" << j << endl;
        }
    }
    os << "Original canvas: " << endl;
    int n = original_canvas.size();
    os << n << endl;
    for (int i=0; i < n; ++i) {
        for (int j : original_canvas[i]) {
            os << i << " " << j << endl;
        }
    }
    os << "---" << endl;
}

void TwoTriangleGraph::write_prolog(std::ostream& os) {
    os << "numVertices(" << g.n + 6 << ")." << endl;
    os << "numVerticesTSubgraph(6)." << endl;
    os << "tEdge(0,1)." << endl;
    os << "tEdge(0,2)." << endl;
    if(path_length == 1) {
        os << "tEdge(0,3)." << endl;
    }
    os << "tEdge(1,2)." << endl;
    os << "tEdge(3,4)." << endl;
    os << "tEdge(3,5)." << endl;
    os << "tEdge(4,5)." << endl;

    for (int i=0; i < g.n; ++i) {
        for (int j : edges_to_triangles[i]) {
            os << "iEdge(" << j << "," << i+6 << ")." << endl;
        }
    }

    for (int i=0; i < g.n; ++i) {
        for (int j : g.al[i]) {
            if (i < j) {
                os << "iEdge(" << i + 6 << "," << j + 6 << ")." << endl;
            }
        }
    }
}

void TwoTriangleGraph::write(std::ostream& os) {
    write_verbose(os);
}

bool test_graph (const Graph& g) {
    
   return !recursive_alon_tarsi(g);
}

vector<TwoTriangleGraph> read_canvas_and_generate_triangle_graphs() {
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
    vector<TwoTriangleGraph> ans;

    for (int s=0; s < l; ++s) {
        int nn = n-l+(path_length-1);
        vector<int> list_sizes(nn, 5);
        vector<vector<int> > nal(nn);
        vector<vector<int> > nam(nn, vector<int> (nn));
        vector<int> morph(n, -1);
        vector<vector<int> > ett (nn);

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

        auto add_ett = [&] (int u, int t) {
            list_sizes[morph[u]]--;
            ett[morph[u]].push_back(t);
        };

        for (int u = 0; u < n; ++u) {
            if (morph[u] == -1) continue;
            if (u >= l) {
                for (int v : al[u]) {
                    if (morph[v] == -1) continue;
                    else if (v < l && v != (morph[v]+1+s)%l && (ral[u].find((morph[v]+1+s)%l) != ral[u].end())) continue;
                    nal[morph[u]].push_back(morph[v]);
                    nam[morph[u]][morph[v]] = 1;
                }
                if (am[u][s] || am[u][(s+2*path_length+3)%l]) add_ett(u, 0);
                if (am[u][(s+path_length)%l] || am[u][(s+path_length+3)%l]) add_ett(u, 3);
                if (am[u][(s+1*path_length+1)%l]) add_ett(u, 4);
                if (am[u][(s+1*path_length+2)%l]) add_ett(u, 5);
                if (am[u][(s+2*path_length+4)%l]) add_ett(u, 1);
                if (am[u][(s+2*path_length+5)%l]) add_ett(u, 2);
            }
            else {
                if (u == (s+1)%l) {
                    add_ett(u, 0); //Assuming no chords
                }
                if (u == (s+path_length-1)%l) {
                    add_ett(u, 3);
                }
            }
        }
        ans.emplace_back(Graph(list_sizes, nal), path_length, al, ett);
        
        
    }

    return ans;
}



void process_canvas(map<GraphCode, TwoTriangleGraph>& critical_set) {
    vector<TwoTriangleGraph> candidate_list = read_canvas_and_generate_triangle_graphs();
    
    //debug(candidate_list.size());

    map<GraphCode, int> candidate_set;

    for (int i=0; i < (int) candidate_list.size(); ++i) {
        GraphCode gc = candidate_list[i].g.compute_code();



        if (candidate_set.find(gc) == candidate_set.end() && critical_set.find(gc) == critical_set.end()) {
            candidate_set[gc] = i; 
        }
    }


    for (auto p : candidate_set) {
        const Graph& g = candidate_list[p.second].g;

       

        if (test_graph(g)) {
            critical_set[p.first] = candidate_list[p.second];
        }
    }

    

   
}

void read_canvases_and_print_critical_triangles() {

    map<GraphCode, TwoTriangleGraph> critical_set;
    int T;
    cin >> T;
    for (int t = 0; t < T; ++t) {
        cout << "Processing canvas " << t << "/" << T << endl;
        process_canvas(critical_set);
    }

    cout << critical_set.size() << endl;

    for (auto x : critical_set) {
        //std::ofstream fout("../criticality_verifier/triangle_graphs/tg_" + x.first.to_string() + ".pl");
        x.second.write(cout);
    }
}

