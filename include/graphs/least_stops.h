#pragma once

#include <vector>
#include <list>
#include <string>
#include <iostream>

class LeastStopsGraph {

   struct SearchInfo {
        int parent;
        int num_stops;

        bool visited;
    };

    struct Edge {
        int dest;
    };

    struct Node {
        std::list<Edge> adj;
        SearchInfo search;
    };

    std::vector<Node> nodes;

    /**
     * @brief Applies Breadth-First Search to the graph in starting node
     * 
     * @param start The starting node
     */
    void bfs(int start);

public:
    /**
    * @brief Creates a graph with n nodes
    * 
    * @param n The number of nodes
    */
    LeastStopsGraph(int n);
    
    /**
     * @brief Adds a new edge to the graph
     * 
     * @param src Starting node
     * @param dest Destination node
     */
    void add_edge(int src, int dest);

    /**
     * @brief Gets the path from the starting node 
     * to the destination node
     * 
     * @param start Starting node
     * @param end Destination node
     * @return std::list<int> List of the nodes traversed
     */
    std::list<int> get_path(int start, int end);
};


