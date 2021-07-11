#ifndef ONLINE_EDGE_DET_H // To make sure you don't declare the function more than once by including the header multiple times.
#define ONLINE_EDGE_DET_H
#include "LEDA/graph/ugraph.h"
#include "LEDA/graph/graph.h"
#include <iostream>
using namespace leda;
struct NodeInfoBFS
{
    bool visited;
    unsigned int arrival;
    string color;
    int level;
};
bool breaks_connected(ugraph &G, edge e, node_array<NodeInfoBFS> &info, node_array<list<edge>> &set_A, node_array<list<edge>> &set_B, node_array<list<edge>> &set_C);

#endif