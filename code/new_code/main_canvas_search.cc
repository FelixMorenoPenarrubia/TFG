#include<iostream>
#include<vector>
#include<string>
#include "Canvas.hh"
#include "CanvasSearch.hh"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

int main() {
    CanvasSearch s;
	int l;
	cin >> l;
	std::set<CanvasCode>& v = s.get_chordless_code(l);

	cout << v.size() << endl;
	for (CanvasCode c : v) {
		cout << c.to_string() << endl;
	}
}