#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<chrono>
#include "Canvas.hh"
#include "TwoTriangleGraph.hh"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

int main() {
    int T;
    cin >> T;
    map<TwoTriangleGraphCode, TwoTriangleGraph> m;

    int max_time = 0;
    long long total_time_50 = 0;
    long long total_time_500 = 0;

    auto begin_time = std::chrono::system_clock::now();

	for (int t=0; t < T; ++t) {

        auto start = std::chrono::system_clock::now();

        
        Canvas c = Canvas::read_code(cin);
       
        
        vector<TwoTriangleGraph> vec = TwoTriangleGraph::generate_from_canvas(c);

        for (TwoTriangleGraph g : vec) {

            g.compute_code();

            if (m.find(g.compute_code()) == m.end()) {
                if (g.test_criticality()) {
                    m[g.compute_code()] = g;
                }
            }
        }

        auto end = std::chrono::system_clock::now();

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        total_time_50 += elapsed.count()/50;
        total_time_500 += elapsed.count()/500;

        if (elapsed.count() > max_time || t % 20000 == 0) {
            if (elapsed.count() > max_time) {
                max_time = elapsed.count();
                cerr << "New maximum time : " << max_time << "ms " << endl;
                cerr << "In canvas: " << c.compute_code().to_string() << endl;
            }
            cerr << "Current statistics: " << endl;
            cerr << "Canvas " << t+1 << "/" << T << endl;
            cerr << "Total time elapsed so far: " << std::chrono::duration_cast<std::chrono::seconds>(end-begin_time).count() << "s" << endl;
            cerr << "Total time elapsed (50ms granularity): " << total_time_50/20 << "s" << endl;
            cerr << "Total time elapsed (500ms granularity): " << total_time_500/2 << "s" << endl;
        }
    }

    
    cout << m.size() << endl;
    for (auto p : m) {
        p.second.write(cout);
    }
    
    /*
    for (auto p : m) {
        std::ofstream fout("../criticality_verifier/new_triangle_graphs/tg_" + p.first.to_string() + ".pl");
        p.second.write_prolog(fout);
    }
    */
}