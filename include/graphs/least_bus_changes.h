#pragma once

#include <vector>
#include <list>
#include <string>
#include <optional>
#include <deque>

class LeastBusChangesGraph {

    struct SearchInfo {
        int parent;

        std::pair<int, int> distance; // [line_changes, bus_stops]
        bool visited;
    };

    struct Edge {
        int dest;
        int weight;
    };

    struct Node {
        std::deque<Edge> adj;
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
    LeastBusChangesGraph(int n);
    
    /**
     * @brief Resizes the graph to a different number of nodes
     * 
     * @param n Number of nodes for the graph to be resized to
     * @param force Boolean that represents whether to force or
     * not the resize of the graph
     */
    void resize(int n, bool force);

    /**
     * @brief Adds a new edge to the graph
     * 
     * @param src Starting node
     * @param dest Destination node
     * @param weight Weight of the new node
     */
    void add_edge(int src, int dest, int weight);

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
