DEBUGFLAGS=-g -fsanitize=undefined,address -Wall -DDEBUG
PERFORMFLAGS=-O2 -Wall
SRC_FILES := $(wildcard *.cpp)

canvas_search: main_canvas_search.cc $(SRC_FILES)
	g++ $(PERFORMFLAGS) -o canvas_search main_canvas_search.cc *.cpp

debug: main_debug.cc $(SRC_FILES)
	g++ $(DEBUGFLAGS) -o debug main_debug.cc *.cpp 


