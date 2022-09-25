DEBUGFLAGS=-g -fsanitize=undefined,address -Wall -DDEBUG
PERFORMFLAGS=-O2 -Wall

search: main_search.cpp search.cpp PlaneGraph.cpp
	g++ $(PERFORMFLAGS) -o search main_search.cpp

debug: main_debug.cpp search.cpp PlaneGraph.cpp
	g++ $(DEBUGFLAGS) -o debug main_debug.cpp


