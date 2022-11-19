#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<chrono>
#include "Canvas.hh"
#include "TwoTriangleGraph.hh"
#include "Parallelism.hh"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

int main() {
    int T;
    cin >> T;
    std::set<TwoTriangleGraphCode> s;

    

	for (int t=0; t < T; ++t) {
        Canvas c = Canvas::read_code(cin);
        vector<TwoTriangleGraph> vec = TwoTriangleGraph::generate_from_canvas(c);
        for (TwoTriangleGraph g : vec) {
            Parallelism::two_triangle_list_mutex.lock();
            bool in_memory = s.find(g.compute_code()) != s.end();
            Parallelism::two_triangle_list_mutex.unlock();
            if (!in_memory) {
                Parallelism::spawn_thread([g, &s] () {
                    if (g.test_criticality()) {
                        Parallelism::two_triangle_list_mutex.lock();
                        bool in_memory = s.find(g.compute_code()) != s.end();
                        if (!in_memory) {
                            s.insert(g.compute_code());
                        }
                        Parallelism::two_triangle_list_mutex.unlock();
                    }
                });
            }
        }

        

        
    }

    Parallelism::wait_for_threads();

    
    cout << s.size() << endl;
    for (auto c : s) {
        cout << c.to_string() << endl;
    }
    
    /*
    for (auto p : m) {
        std::ofstream fout("../criticality_verifier/new_triangle_graphs/tg_" + p.first.to_string() + ".pl");
        p.second.write_prolog(fout);
    }
    */
}