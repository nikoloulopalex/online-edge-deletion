#include "LEDA/graph/graph.h"
#include "LEDA/graph/ugraph.h"
#include <LEDA/graph/graph_gen.h>
#include "LEDA/graph/graph_alg.h"
#include "LEDA/system/timer.h"
#include "bipartite.h"
#include "online_edge_del.h"
#include "online_edge_deletion.hpp"
#include "graph_gen.h"
#include <iostream>
using namespace leda;

int main()
{
    ugraph U;

    timer my_timer;

    // ----------------- Connceted Squares graph Graph ---------------------//
    int n = 100;
    std::cout << "First Family of Graphs with " << 4 * n << "nodes"
              << "\n";
    U = connected_squares(n);

    // ----------------- Syntetic Graph ---------------------//
    // int n = 100;
    // std::cout << "Second Family of Graphs with " << 4 * n << "nodes"
    //           << "\n";
    // U = synth_graph(n);

    // ----------------- Random Graph ---------------------//
    // int n = 10000;
    // int m = 10000;
    // std::cout << "Third Family of Graphs with " <<  n << "nodes"
    //           << "\n";
    // graph G;
    // random_graph(G, n, m);
    // G.make_undirected();
    // U = G;

    edge e;
    node v;
    edge delete_edge;
    node_array<int> number(U);
    int count = 1;

    // -----------------info arrays-----//
    // node array used for node informatione such as color
    node_array<NodeInfoBFS> info(U);
    // edges that belong to the previous level
    node_array<list<edge>> set_A(U);
    // edges that belong to the same level
    node_array<list<edge>> set_B(U);
    // edges that belong to the next level
    node_array<list<edge>> set_C(U);

    my_timer.start();
    delete_edge = U.choose_edge();
    breaks_connected(U, delete_edge, info, set_A, set_B, set_C);

    my_timer.stop();
    std::cout << "My time " << my_timer.elapsed_time() << "\n";
    delete_edge = U.choose_edge();

    my_timer.start();
    breaks_connected(U, delete_edge, info, set_A, set_B, set_C);
    my_timer.stop();
    std::cout << "My time " << my_timer.elapsed_time() << "\n";

    delete_edge = U.choose_edge();
    my_timer.start();
    breaks_connected(U, delete_edge, info, set_A, set_B, set_C);
    my_timer.stop();
    std::cout << "My time " << my_timer.elapsed_time() << "\n";

    delete_edge = U.choose_edge();
    my_timer.start();
    breaks_connected(U, delete_edge, info, set_A, set_B, set_C);
    my_timer.stop();
    std::cout << "My time " << my_timer.elapsed_time() << "\n";
}