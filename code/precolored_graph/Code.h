#include<vector>
#include<map>
#include<iostream>
#include<string>

#ifndef CODE_H_
#define CODE_H_



using std::vector;
using std::string;

struct GraphCode {
	vector<int> code;
	
	GraphCode() {
	
	}
	
	void push_r(int x);
	
	void push_f(int list_size);
	
	void push_b();
	
	int size() const;
	
	string to_string() const;
	int operator[](int i) const;

	
};


bool operator<(const GraphCode& a, const GraphCode& b);



#endif