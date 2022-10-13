#include<vector>
#include<map>
#include "Graph.h"
#ifndef ALONTARSI_H_
#define ALONTARSI_H_

using std::vector;
using std::map;


vector<int> minimal_irreducible_deletedvertices(const Graph& g);

bool alon_tarsi(const Graph& g);

bool recursive_alon_tarsi(const Graph& g);

#endif