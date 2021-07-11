#include "LEDA/graph/graph.h"
#include "LEDA/graph/ugraph.h"
#include "LEDA/graph/basic_graph_alg.h"
#include "LEDA/graph/graph_alg.h"
#include "LEDA/graph/node_set.h"
#include "./online_edge_del.h"
#include "LEDA/core/queue.h"
#include <array>
#include <iostream>
unsigned int counterBFS = 1;
bool initialized = false;
using namespace leda;

struct NodeInfoDFS
{
    bool visited;
};

void DFS(ugraph &G, node source, node target, node_array<NodeInfoDFS> &info)
{
    info[source].visited = true;
    node v;
    list<node> adjnodes = G.adj_nodes(source);
    forall(v, adjnodes)
    {
        if (!info[v].visited)
        {
            DFS(G, v, target, info);
        }
    }
}

node find_other_components(ugraph &G, node_array<NodeInfoBFS> &info)
{
    node r;
    forall_nodes(r, G)
    {
        if (!info[r].visited)
            return r;
    }
    return G.first_node();
}

// BFS algorithm  with coloring implementation for nodes
list<node> my_BFS(ugraph &G, node s, node_array<NodeInfoBFS> &info, int init_level)
{
    // info for 1st node
    info[s].visited = true;
    info[s].arrival = counterBFS;
    info[s].level = init_level;
    node v;
    node r;
    edge e;
    // the list used for the route that BFS followed
    list<node> node_list;
    // The queue used for BFS
    queue<node> q;
    q.push(s);
    node_list.push(s);

    // Run while there are vertices
    while (!q.empty())
    {
        node u = q.pop();
        forall_adj_edges(e, u)
        {
            // Node v is the node connected to node u by edge e
            v = G.opposite(e, u);
            if (!info[v].visited)
            {
                info[v].visited = true;
                info[v].arrival = counterBFS++;
                info[v].level = info[u].level + 1;
                // push node to queue for later examination about adj edges and to node list for the route BFS followed
                q.append(v);
                node_list.push(v);
            }
        }
    }

    r = find_other_components(G, info);
    if (!info[r].visited)
    {
        my_BFS(G, r, info, 1);
    }

    return node_list;
}

ugraph maintain_structure(ugraph &G, node v, node_array<NodeInfoBFS> &info, node_array<list<edge>> &set_A, node_array<list<edge>> &set_B, node_array<list<edge>> &set_C)
{
    // The queue used for vertices, whose level must be changed
    queue<node> q;
    q.push(v);
    while (!q.empty())
    {

        node w = q.pop();
        info[w].level += 1;

        edge e_b;
        forall(e_b, set_B[w])
        {

            node w_ = G.opposite(e_b, w);

            // move edge for the opposite node from set B to set C
            set_B[w_].remove(e_b);
            set_C[w_].push(e_b);
        }

        // move all edges from setB to set A
        set_A[w] = set_B[w];

        edge e_c;
        forall(e_c, set_C[w])
        {

            node w_opp = G.opposite(e_c, w);

            // move edge for the opposite node from set B to set C
            set_A[w_opp].remove(e_c);
            set_B[w_opp].push(e_c);
            if (set_A[w_opp].empty())
            {

                q.push(w_opp);
            }
        }

        set_B[w] = set_C[w];
        set_C[w].clear();

        if (set_A[w].empty())
        {
            q.push(w);
        }
    }
    return G;
}

void find_edge_sets(ugraph &G, node_array<NodeInfoBFS> &info, node_array<list<edge>> &set_A, node_array<list<edge>> &set_B, node_array<list<edge>> &set_C)
{
    node v;

    forall_nodes(v, G)
    {

        edge e;
        forall_adj_edges(e, v)
        {
            node opposite = G.opposite(e, v);
            switch (info[opposite].level - info[v].level)
            {
                // edges that belong to the previous level
            case -1:
                set_A[v].push(e);
                break;
                // edges that belong to the same level
            case 0:
                set_B[v].push(e);
                break;
                // edges that belong to the next level
            case 1:
                set_C[v].push(e);
                break;
            }
        }
    }
}

bool process_A(ugraph &G, node source, node target, edge e)
{
    bool breaks_component;
    node_array<NodeInfoDFS> info(G);
    node v;
    forall_nodes(v, G)
    {
        info[v].visited = false;
    }
    G.hide_edge(e);
    DFS(G, source, target, info);
    G.restore_edge(e);
    breaks_component = (info[target].visited) ? false : true;

    return breaks_component;
}

ugraph process_B(ugraph &G, node source, node target, edge e, node_array<NodeInfoBFS> &info, node_array<list<edge>> &set_A, node_array<list<edge>> &set_B, node_array<list<edge>> &set_C)
{
    // Delete edge from graph
    G.del_edge(e);

    // Case 1 -- Nodes are on the same level
    if (info[source].level == info[target].level)
    {
        std::cout << "--Case 1-- Both nodes are on same level so we can delete edge safely" << std::endl;
        set_B[source].remove(e);
        set_B[target].remove(e);

        return G;
    }
    else
    {
        // case 2 -- Nodes are not on the same  level
        std::cout << "--Case 2-- Nodes are not on the same level" << std::endl;
        set_C[source].remove(e);
        set_A[target].remove(e);

        if (!set_A[target].empty())
        {
            // case 2.1 --- set A not empty so edge deletion does not break a component
            std::cout << "--Case 2.1-- setA not empty so we can return  graph without broke components" << std::endl;
            return G;
        }

        // case 2.2 ----- setA is empty so I have to maintain graph structure
        std::cout << "--Case 2.2-- setA empty so I have to maintain graph structure" << std::endl;

        maintain_structure(G, target, info, set_A, set_B, set_C);
    }

    return G;
}

bool breaks_connected(ugraph &G, edge e, node_array<NodeInfoBFS> &info, node_array<list<edge>> &set_A, node_array<list<edge>> &set_B, node_array<list<edge>> &set_C)
{
    // Initialize BFS structure
    if (!initialized)
    {
        std::cout << "----- Initialize BFS structure -------- " << std::endl;
        node s = G.first_node();

        // run BFS algorithm and color nodes
        list<node> BFS_nodes = my_BFS(G, s, info, 0);

        // Setup setA,B,C of edges for every node
        find_edge_sets(G, info, set_A, set_B, set_C);
        initialized = true;
    }

    std::cout << "Is this edge breaking connectivity " << std::endl;
    G.print_edge(e);
    std::cout << std::endl;
    node source = G.source(e);
    node target = G.target(e);
    bool breaks_component_A = process_A(G, source, target, e);
    if (!breaks_component_A)
    {
        process_B(G, source, target, e, info, set_A, set_B, set_C);
        return false;
    }
    else
    {
        std::cout << "--Halted-- Process A detected component break " << std::endl;
        // Delete edge from graph , Info and Sets remain the same
        G.del_edge(e);
        return true;
    }
}
