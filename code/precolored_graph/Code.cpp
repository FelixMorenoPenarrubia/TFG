#include "Code.h"



void Code::push_r(int x) {
    code.push_back(1+x);
}

void Code::push_f(int list_size) {
    code.push_back(-1-list_size);
}

void Code::push_b() {
    code.push_back(0);
}

int Code::size() const {
    return code.size();
}

string Code::to_string() const { 
    string s = "";
    for (int i=0; i < size(); ++i) {
        if (code[i] < 0) {
            s.push_back('F');
            s.push_back('0'-code[i]+1);
        }
        else if (code[i] > 0) { 
            s.push_back('R');
            s.push_back('A'+code[i]-1);
        }
        else {
            s.push_back('B');
        }
    }
    return s;
}
int Code::operator[](int i) const {
    return code[i];
}

bool operator<(const Code& a, const Code& b) {
	for (int i=0; i < std::min(a.size(), b.size()); ++i) {
		if (a[i] < b[i]) return true;
		if (a[i] > b[i]) return false;
	}
	return a.size() < b.size();
}