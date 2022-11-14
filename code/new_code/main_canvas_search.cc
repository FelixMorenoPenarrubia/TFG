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
	vector<Canvas> v = s.get_chordless(l);

	cout << v.size() << endl;
	for (Canvas g : v) {
		cout << g.compute_code().to_string() << endl;
		//g.write(cout);
	}
}