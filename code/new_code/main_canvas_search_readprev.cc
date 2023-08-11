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
    s.read_previous_and_print_lessmemory();
}