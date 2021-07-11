#include "LEDA/graph/graph.h"
#include "LEDA/graph/ugraph.h"
#include <LEDA/graph/graph_gen.h>
#include "LEDA/graph/graph_alg.h"
#include "LEDA/graph/node_set.h"
#include "graph_gen.h"
#include <array>
#include <iostream>

using namespace leda;

// a graph generator for nested squares graphs
ugraph connected_squares(int n)
{
    std::cout << "Creating Connected Squares Graph ... "
              << "\n";

    ugraph G;
    // create n nodes
    for (int i = 0; i < 4 * n; i++)
        G.new_node();
    node v;
    // a node array to hold numbers of every node
    node_array<int> number(G);
    int count = 1;
    // assign every node a number 1 to n
    forall_nodes(v, G)
    {
        number[v] = count++;
    }
    // make the desired connections for a nested squares graph
    forall_nodes(v, G)
    {
        node k;
        forall_nodes(k, G)
        {
            // connect every node with the node of the next square( ex. node 1 with node 4)
            if (number[k] - number[v] == 3 && number[v] % 2 == 0 && number[v] % 4 != 0)
            {
                G.new_edge(v, k);
            }
            // connect every node with the node of the  square except the last with its nest( ex. node 1 with node 2, node 2 with node 3)
            if (number[v] % 4 != 0 && number[k] - number[v] == 1)
            {
                G.new_edge(v, k);
            }
            // connect last node of the square with the first of square( ex. node 4 with node 1)
            if (number[v] % 4 == 0 && number[v] - number[k] == 3)
            {
                G.new_edge(v, k);
            }
        }
    }
    // return undirected graph G
    std::cout << "Connected Squares Graph is ready for benchmark!"
              << "\n";
    return G;
}

// a graph generator for 4 levels synthetic graphs
ugraph synth_graph(int n)
{
    std::cout << "Creating Synthetic Graph ... "
              << "\n";

    ugraph G;
    int number_of_nodes = 4 * n;
    node l1_nodes[n];
    node l2_nodes[n];
    node l3_nodes[n];
    node l4_nodes[n];

    // create n nodes
    for (int i = 1; i <= 4 * n; i++)
    {
        G.new_node();
    }
    node v;
    // a node array to hold numbers of every node
    node_array<int> number(G);
    node_array<int> level(G);

    int count = 1;
    int current_level = 1;
    // // assign every node a number 1 to n and a level 1 to 4
    forall_nodes(v, G)
    {
        number[v] = count++;
        level[v] = current_level;
        switch (level[v])
        {
        case 1:
            l1_nodes[count - 2] = v;
            break;
        case 2:
            l2_nodes[count - 2] = v;
            break;
        case 3:
            l3_nodes[count - 2] = v;
            break;
        case 4:
            l4_nodes[count - 2] = v;
            break;
        }
        if (count == n + 1 || count == 2 * n + 1 || count == 3 * n + 1 || count == 4 * n + 1)
        {
            current_level++;
            count = 1;
        }
    }

    // make the desired connections for a synthetic graph with 4 level
    forall_nodes(v, G)
    {
        node k;
        forall_nodes(k, G)
        {

            // connect every node with the same node of the next level
            if (number[k] == number[v] && level[k] - level[v] == 1)
            {
                G.new_edge(v, k);
            }
        }
    }

    int j = 1;
    while (j < 4)
    {
        node l;
        l = G.choose_node();
        switch (j)
        {
        case 1:
            l = l1_nodes[rand() % n];
            break;
        case 2:
            l = l2_nodes[rand() % n];
            break;
        case 3:
            l = l3_nodes[rand() % n];
            break;
        case 4:
            l = l4_nodes[rand() % n];
            break;
        }
        node k;
        forall_nodes(k, G)
        {

            // connect random node with all the nodes of the next level
            if (level[k] - level[l] == 1)
            {
                G.new_edge(l, k);
            }
        }
        j++;
    }

    // connect random node from level 1 with random node from level 3 and (2,4)
    node l1;
    l1 = l1_nodes[rand() % n];
    node l2;
    l2 = l2_nodes[rand() % n];
    node l3;
    l3 = l3_nodes[rand() % n];
    node l4;
    l4 = l4_nodes[rand() % n];

    G.new_edge(l1, l3);
    G.new_edge(l2, l4);

    // return undirected graph G
    std::cout
        << "Synthetic Graph is ready for benchmark!"
        << "\n";
    return G;
}
