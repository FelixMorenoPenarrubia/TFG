#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<set>
#include "TwoTriangleGraph.hh"
#include "GlobalSettings.hh"
#include "ReducibilityTests.hh"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::string;


bool fivecolorable(const ListGraph& g) {
    vector<int> f (g.n, -1); 
    int cv = 0; //current vertex of g that we are coloring

    auto test_vertex = [&] () -> bool {
        for (int cu = 0; cu < cv; ++cu) {
            if (f[cu] == f[cv] && g.neighbors(cu, cv)) return false;
        }
        return true;
    };
    while (cv != -1) {
        if (cv == g.n) {
            return true;
        }
        f[cv]++;
        while (f[cv] < 5 && !test_vertex()) {
            f[cv]++;
        }
        if (f[cv] == 5) {
            f[cv] = -1;
            cv--;
        }
        else {
            cv++;
        }

    }
    return false;
}

int main() {
    int T;
    cin >> T;
    vector<TwoTriangleGraph> v;
    for (int i=0; i < T; ++i) {
        v.push_back(TwoTriangleGraph::read(cin));
    }

    for (auto g : v) {
        for (auto gt : g.identify_triangles()) {
            if (!batch_test(gt)) {
                
                vector<Graph> sub = {
                    Graph(
                        {
                            {1, 2, 3, 4, 5},
                            {0, 2, 3, 4, 5},
                            {0, 1, 3, 4, 5},
                            {0, 1, 2, 4, 5},
                            {0, 1, 2, 3, 5},
                            {0, 1, 2, 3, 4}
                        }
                    ),
                    Graph(
                        {
                            {   1, 2, 3, 4, 5, 6, 7},
                            {0,    2, 3, 4, 5, 6, 7},
                            {0, 1,    3, 4, 5, 6, 7},
                            {0, 1, 2,    4,       7},
                            {0, 1, 2, 3,    5      },
                            {0, 1, 2,    4,    6   },
                            {0, 1, 2,       5,    7},
                            {0, 1, 2, 3,       6   },
                        }
                    ),
                    Graph(
                        {
                            {   1, 2, 3, 4, 5, 6, 7, 8},
                            {0,    2, 3, 4, 5, 6, 7, 8},
                            {0, 1,    3,       6, 7   },
                            {0, 1, 2,    4,       7   },
                            {0, 1,    3,    5,    7, 8},
                            {0, 1,       4,    6,    8},
                            {0, 1, 2,       5,       8},
                            {0, 1, 2, 3, 4            },
                            {0, 1,       4, 5, 6      },
                        }
                    ),
                    Graph(
                        {
                            {1, 2, 3, 8, 9, 10},
                            {9, 10, 0, 2, 3, 4},
                            {0, 1, 3, 4, 5, 10},
                            {0, 1, 2, 4, 5, 6},
                            {1, 2, 3, 5, 6, 7},
                            {2, 3, 4, 6, 7, 8},
                            {3, 4, 5, 7, 8, 9},
                            {4, 5, 6, 8, 9, 10},
                            {5, 6, 7, 9, 10, 0},
                            {6, 7, 8, 10, 0, 1},
                            {7, 8, 9, 0, 1, 2},
                        }
                    )
                };
                
                bool good = false;
                for (auto h : sub) {
                    if (gt.subgraph_isomorphism(h)) {
                        good = true;
                        break;
                    }
                }
                if (!good) {
                    cout << "From TTG :" << endl;
                    g.write(cout);
                    cout << "Graph: " << endl;
                    gt.write_cpp(cout);
                    cout << "---" << endl;
                }
            }
        }
    }
}