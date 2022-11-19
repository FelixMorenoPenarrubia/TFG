#ifndef REDUCIBILITYTESTS_HH_
#define REDUCIBILITYTESTS_HH_

#include "ListGraph.hh"

bool degree_test(const ListGraph& g);

bool alon_tarsi_test(const ListGraph& g);

bool batch_reducible_test(const ListGraph& g);

bool batch_colorable_test(const ListGraph& g);


std::vector<int> minimal_irreducible_deletedvertices(const ListGraph& g);

bool recursive_colorability_test(const ListGraph& g);

bool batch_test(const ListGraph& g);

#endif