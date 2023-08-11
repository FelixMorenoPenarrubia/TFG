#include<iostream>
#include<vector>
#include<string>
#include<set>
#include "Canvas.hh"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

int main() {

    string s;
    int n = 0;
    int cnt = 0;
    while (cin >> s) {
        n = std::max(n, Canvas(CanvasCode(s)).n);
        cnt++;
    }
    cout << cnt << endl << n << endl;
}