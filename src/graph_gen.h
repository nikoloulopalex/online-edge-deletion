#ifndef GRAPH_GEN_H // To make sure you don't declare the function more than once by including the header multiple times.
#define GRAPH_GEN_H
#include "LEDA/graph/ugraph.h"
#include <iostream>
using namespace leda;

ugraph connected_squares(int n);
ugraph synth_graph(int n);

#endif