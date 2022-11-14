#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include "Canvas.hh"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

int main() {
    int T;
    cin >> T;
    cout << T << endl;
    for (int t=0; t < T; ++t) {
        string s;
        cin >> s;
        Canvas(CanvasCode(s)).write(cout);
    }

    
}