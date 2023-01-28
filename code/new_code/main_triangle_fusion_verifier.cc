#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<set>
#include "TwoTriangleGraph.hh"
#include "GlobalSettings.hh"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

int main() {
    GlobalSettings::ALON_TARSI_MEMOIZE = true;
    int T;
    cin >> T;
    vector<TwoTriangleGraph> v;
    std::set<TwoTriangleGraphCode> s;
    for (int i=0; i < T; ++i) {
        v.push_back(TwoTriangleGraph::read(cin));
        s.insert(v.back().compute_code());
    }

    for (auto g1 : v) {
        for (auto g2 : v) {
            vector<TwoTriangleGraph> fus = TwoTriangleGraph::fuse_triangles(g1, g2);
            for (auto g3 : fus) {
                if (s.find(g3.compute_code()) == s.end()) {
                    if (g3.test_criticality()) {
                        cout << "---" << endl;
                        g1.write(cout);
                        g2.write(cout);
                        cout << "-" << endl;
                        g3.write(cout);
                        cout << "---" << endl;
                    }
                }
            }
        }
    }
}