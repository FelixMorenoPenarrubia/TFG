DEBUGFLAGS=-g -fsanitize=undefined,address -Wall -DDEBUG
DEBUGGERFLAGS=-g -Wall -DDEBUG
PERFORMFLAGS=-O2 -Wall
PARALLELFLAGS=-O2 -Wall -DPARALLEL -pthread 
PARALLELDEBUGFLAGS=-O2 -Wall -DPARALLEL -pthread -g -fsanitize=thread
SRC_FILES := $(wildcard *.cpp)

canvas_search: main_canvas_search.cc $(SRC_FILES)
	g++ $(PERFORMFLAGS) -o canvas_search main_canvas_search.cc *.cpp

pp_search: main_precolored_path_graph_search.cc $(SRC_FILES)
	g++ $(PERFORMFLAGS) -o pp_search main_precolored_path_graph_search.cc *.cpp

ppv_search: main_precolored_path_and_vertex_graph_search.cc $(SRC_FILES)
	g++ $(PERFORMFLAGS) -o ppv_search main_precolored_path_and_vertex_graph_search.cc *.cpp

tpp_search: main_two_precolored_paths_search.cc $(SRC_FILES)
	g++ $(PERFORMFLAGS) -o tpp_search main_two_precolored_paths_search.cc *.cpp

tpp_verifier: main_two_precolored_paths_verifier.cc $(SRC_FILES)
	g++ $(PERFORMFLAGS) -o tpp_verifier main_two_precolored_paths_verifier.cc *.cpp

ppv_verifier: main_precolored_path_and_vertex_verifier.cc $(SRC_FILES)
	g++ $(PERFORMFLAGS) -o ppv_verifier main_precolored_path_and_vertex_verifier.cc *.cpp

pp_search_prolog: main_precolored_path_graph_search_prolog.cc $(SRC_FILES)
	g++ $(PERFORMFLAGS) -o pp_search_prolog main_precolored_path_graph_search_prolog.cc *.cpp

canvas_search_lessmemory: main_canvas_search_lessmemory.cc $(SRC_FILES)
	g++ $(PERFORMFLAGS) -o canvas_search_lessmemory main_canvas_search_lessmemory.cc *.cpp

canvas_search_parallel: main_canvas_search.cc $(SRC_FILES)
	g++ $(PARALLELFLAGS) -o canvas_search_parallel main_canvas_search.cc *.cpp

canvas_search_parallel_debug: main_canvas_search.cc $(SRC_FILES)
	g++ $(PARALLELDEBUGFLAGS) -o canvas_search_parallel_debug main_canvas_search.cc *.cpp

canvas_search_lessmemory_parallel: main_canvas_search_lessmemory.cc $(SRC_FILES)
	g++ $(PARALLELFLAGS) -o canvas_search_lessmemory_parallel main_canvas_search_lessmemory.cc *.cpp

canvas_search_lessmemory_parallel_debug: main_canvas_search_lessmemory.cc $(SRC_FILES)
	g++ $(PARALLELDEBUGFLAGS) -o canvas_search_lessmemory_parallel_debug main_canvas_search_lessmemory.cc *.cpp

canvas_to_triangles: main_canvas_to_triangles.cc $(SRC_FILES)
	g++ $(PERFORMFLAGS) -o canvas_to_triangles main_canvas_to_triangles.cc *.cpp

canvas_to_triangles_parallel: main_canvas_to_triangles_parallel.cc $(SRC_FILES)
	g++ $(PARALLELFLAGS) -o canvas_to_triangles_parallel main_canvas_to_triangles_parallel.cc *.cpp

canvas_to_triangles_parallel_debug: main_canvas_to_triangles_parallel.cc $(SRC_FILES)
	g++ $(PARALLELDEBUGFLAGS) -o canvas_to_triangles_parallel_debug main_canvas_to_triangles_parallel.cc *.cpp

unpack_canvases: main_unpack_canvases.cc $(SRC_FILES)
	g++ $(PERFORMFLAGS) -o unpack_canvases main_unpack_canvases.cc *.cpp

unpack_twotrianglegraphs: main_unpack_twotrianglegraphs.cc $(SRC_FILES)
	g++ $(PERFORMFLAGS) -o unpack_twotrianglegraphs main_unpack_twotrianglegraphs.cc *.cpp

sort_codes: main_sort_codes.cc $(SRC_FILES)
	g++ $(PERFORMFLAGS) -o sort_codes main_sort_codes.cc *.cpp

sort_wedges: main_sort_wedges.cc $(SRC_FILES)
	g++ $(PERFORMFLAGS) -o sort_wedges main_sort_wedges.cc *.cpp

sort_biwedges: main_sort_biwedges.cc $(SRC_FILES)
	g++ $(PERFORMFLAGS) -o sort_biwedges main_sort_biwedges.cc *.cpp

bucket_biwedges: main_bucket_biwedges.cc $(SRC_FILES)
	g++ $(PERFORMFLAGS) -o bucket_biwedges main_bucket_biwedges.cc *.cpp

compare_pp: main_compare_precoloredpathgraphs.cc $(SRC_FILES)
	g++ $(PERFORMFLAGS) -o compare_pp main_compare_precoloredpathgraphs.cc *.cpp

debug_sanitizer: main_debug.cc $(SRC_FILES)
	g++ $(DEBUGFLAGS) -o debug main_debug.cc *.cpp 

debug_debugger: main_debug.cc $(SRC_FILES)
	g++ $(DEBUGGERFLAGS) -o debug_debugger main_debug.cc *.cpp 


