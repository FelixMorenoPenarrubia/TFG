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
    int T;
    cin >> T;
    vector<TwoTriangleGraph> v;
    vector<TwoTriangleGraph> ans;
    for (int i=0; i < T; ++i) {
        auto ttg = TwoTriangleGraph::read(cin);
        if(!ttg.has_separating_triangle()) {
            ans.push_back(ttg);
        }
    }

    cout << ans.size() << endl;

    for (auto ttg : ans) {
        ttg.write(cout);
    }

   
}