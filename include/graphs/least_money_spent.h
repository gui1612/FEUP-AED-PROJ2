#pragma once

#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <unordered_set>

class LeastMoneySpentGraph {

   struct SearchInfo {
        int parent;
        std::unordered_set<std::string> used_zones;

        bool visited;
    };

    struct Edge {
        int dest;
    };

    /**
     * @brief Represents a graph's node
     * 
     */
    struct Node {
        std::list<Edge> adj;

        std::string zone;
        SearchInfo search;
    };

    std::vector<Node> nodes;

    /**
     * @brief Applies Dijkstra's algorithm
     * 
     * @param start The starting node
     */
    void dijkstra(int start);
    
public:
    /**
    * @brief Creates a graph with n nodes divided in zones
    * 
    * @param n The number of nodes
    * @param zones The available zones
    */
    LeastMoneySpentGraph(int n, std::vector<std::string> zones);
    
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
    std::list<int> get_path(int start, int dest);
};


