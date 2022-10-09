DEBUGFLAGS=-g -fsanitize=undefined,address -Wall -DDEBUG
PERFORMFLAGS=-O2 -Wall

debug: main_debug.cpp Graph.cpp CanvasToTriangles.cpp Code.cpp AlonTarsi.cpp debug_utility.cpp
	g++ $(DEBUGFLAGS) -o debug debug_utility.cpp main_debug.cpp Graph.cpp CanvasToTriangles.cpp Code.cpp AlonTarsi.cpp 
perform: main_debug.cpp Graph.cpp CanvasToTriangles.cpp Code.cpp AlonTarsi.cpp debug_utility.cpp
	g++ $(PERFORMFLAGS) -o perform debug_utility.cpp main_debug.cpp Graph.cpp CanvasToTriangles.cpp Code.cpp AlonTarsi.cpp


