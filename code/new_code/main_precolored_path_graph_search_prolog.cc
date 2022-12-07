#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include "PrecoloredPathGraph.hh"
#include "PrecoloredPathGraphSearch.hh"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

int main() {
    PrecoloredPathGraphSearch s;
	int l;
	cin >> l;
	vector<PrecoloredPathGraph> v = s.get_all(l);
    
	for (PrecoloredPathGraph g : v) {
		std::ofstream fout("../criticality_verifier/wedges/wed_" + g.compute_code().to_string() + ".pl");
        g.write_prolog(fout);
	}
}