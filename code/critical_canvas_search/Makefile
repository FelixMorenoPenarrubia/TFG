DEBUGFLAGS=-g -fsanitize=undefined,address -Wall -DDEBUG
PERFORMFLAGS=-O2 -Wall

search: main_search.cpp search.cpp PlaneGraph.cpp search_config.cpp
	g++ $(PERFORMFLAGS) -o search main_search.cpp ../precolored_graph/Graph.cpp ../precolored_graph/AlonTarsi.cpp ../precolored_graph/Code.cpp

debug: main_debug.cpp search.cpp PlaneGraph.cpp search_config.cpp
	g++ $(DEBUGFLAGS) -o debug main_debug.cpp ../precolored_graph/Graph.cpp ../precolored_graph/AlonTarsi.cpp ../precolored_graph/Code.cpp


