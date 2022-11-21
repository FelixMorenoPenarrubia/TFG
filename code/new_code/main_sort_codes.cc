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
    
	std::set<CanvasCode> v;

    string s;
    while (cin >> s) {
        v.insert(CanvasCode(s));
    }

	cout << v.size() << endl;
	for (CanvasCode c : v) {
		cout << c.to_string() << endl;
	}
}