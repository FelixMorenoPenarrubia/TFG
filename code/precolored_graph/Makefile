DEBUGFLAGS=-g -fsanitize=undefined,address -Wall -DDEBUG
PERFORMFLAGS=-O2 -Wall

debug: main_debug.cpp Graph.cpp CanvasToTriangles.cpp Code.cpp AlonTarsi.cpp debug_utility.cpp
	g++ $(DEBUGFLAGS) -o debug debug_utility.cpp main_debug.cpp Graph.cpp CanvasToTriangles.cpp Code.cpp AlonTarsi.cpp 
canvas_to_triangles: main.cpp Graph.cpp CanvasToTriangles.cpp Code.cpp AlonTarsi.cpp debug_utility.cpp
	g++ $(PERFORMFLAGS) -o canvas_to_triangles debug_utility.cpp main.cpp Graph.cpp CanvasToTriangles.cpp Code.cpp AlonTarsi.cpp


