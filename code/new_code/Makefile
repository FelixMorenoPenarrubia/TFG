DEBUGFLAGS=-g -fsanitize=undefined,address -Wall -DDEBUG
DEBUGGERFLAGS=-g -Wall -DDEBUG
PERFORMFLAGS=-O2 -Wall
PARALLELFLAGS=-O2 -Wall -DPARALLEL -pthread 
PARALLELDEBUGFLAGS=-O2 -Wall -DPARALLEL -pthread -g -fsanitize=thread
SRC_FILES := $(wildcard *.cpp)

canvas_search: main_canvas_search.cc $(SRC_FILES)
	g++ $(PERFORMFLAGS) -o canvas_search main_canvas_search.cc *.cpp

canvas_search_parallel: main_canvas_search.cc $(SRC_FILES)
	g++ $(PARALLELFLAGS) -o canvas_search_parallel main_canvas_search.cc *.cpp

canvas_search_parallel_debug: main_canvas_search.cc $(SRC_FILES)
	g++ $(PARALLELDEBUGFLAGS) -o canvas_search_parallel_debug main_canvas_search.cc *.cpp

canvas_to_triangles: main_canvas_to_triangles.cc $(SRC_FILES)
	g++ $(PERFORMFLAGS) -o canvas_to_triangles main_canvas_to_triangles.cc *.cpp

canvas_to_triangles_parallel: main_canvas_to_triangles_parallel.cc $(SRC_FILES)
	g++ $(PARALLELFLAGS) -o canvas_to_triangles_parallel main_canvas_to_triangles_parallel.cc *.cpp

canvas_to_triangles_parallel_debug: main_canvas_to_triangles_parallel.cc $(SRC_FILES)
	g++ $(PARALLELDEBUGFLAGS) -o canvas_to_triangles_parallel_debug main_canvas_to_triangles_parallel.cc *.cpp

unpack_canvases: main_unpack_canvases.cc $(SRC_FILES)
	g++ $(PERFORMFLAGS) -o unpack_canvases main_unpack_canvases.cc *.cpp

debug_sanitizer: main_debug.cc $(SRC_FILES)
	g++ $(DEBUGFLAGS) -o debug main_debug.cc *.cpp 

debug_debugger: main_debug.cc $(SRC_FILES)
	g++ $(DEBUGGERFLAGS) -o debug_debugger main_debug.cc *.cpp 


