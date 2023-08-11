#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<set>

using namespace std;

const int FILE_SIZE = 1'000'000;
int n_file = 0;

void dump_into_file(const vector<string>& v) {
    ofstream fout("/media/felix/T7/crit14_partitioned/crit14_" + to_string(10*(n_file/10)) + "_" + to_string(10*(n_file/10)+9) + "/crit14_" + to_string(n_file) + ".txt");
    for (string s : v) {
        fout << s << endl;
    }
    n_file++;
}

int main() {
    
	vector<string> v;

    string s;
    while (cin >> s) {
        v.push_back(s);
        if ((int)v.size() == FILE_SIZE) {
            dump_into_file(v);
            v.clear();
        }
    }
    if(!v.empty()) {
        dump_into_file(v);
        v.clear();
    }

	
}