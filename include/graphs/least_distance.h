#pragma once

#include <vector>
#include <list>
#include <string>

class LeastDistanceGraph {

    /**
     * @brief Holds graph data
     * 
     */
    struct SearchInfo {
        int parent;
        double distance;

        bool visited;
    };

    /**
     * @brief Represents a graph's edge
     * 
     */
    struct Edge {
        int dest;

        double distance;
    };

    /**
     * @brief Represent's a graph's node
     * 
     */
    struct Node {
        std::list<Edge> adj;
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
    * @brief Creates a graph with n nodes
    * 
    * @param n The number of nodes
    */
    LeastDistanceGraph(int n);
    
    /**
     * @brief Adds a new edge to the graph
     * 
     * @param src Starting node
     * @param dest Destination node
     * @param distance distance of the new node
     */
    void add_edge(int src, int dest, double distance);

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
