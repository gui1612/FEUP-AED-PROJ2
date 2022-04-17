#include "graphs/least_distance.h"

#include <stdexcept>
#include "min_heap.h"

using namespace std;

#define INF 1000000

LeastDistanceGraph::LeastDistanceGraph(int n) : nodes(n) {}

void LeastDistanceGraph::add_edge(int src, int dest, double distance) {
    if (src < 0 || dest < 0 || src >= nodes.size()  || dest >= nodes.size())
        throw invalid_argument("src or dest out of bounds");

    for (const Edge &edge : nodes[src].adj)
        if (edge.dest == dest)
            return;

    nodes[src].adj.push_back({ dest, distance });
}

void LeastDistanceGraph::dijkstra(int start) {
    for (int i = 0; i < nodes.size(); i++) {
        Node &node = nodes.at(i);

        node.search.parent = -1;
        node.search.distance = INF;
        node.search.visited = false;
    }
    
    nodes[start].search.distance = 0;

    MinHeap<int, int> h(nodes.size(), -42);
    h.insert(start, 0);

    while (h.get_size() > 0) {
        int node = h.remove_min();

        SearchInfo &node_search = nodes[node].search;
        node_search.visited = true;

        for (const Edge& edge : nodes[node].adj) {
            SearchInfo &dest_search = nodes[edge.dest].search;

            if (!dest_search.visited && node_search.distance + edge.distance < dest_search.distance) {
                dest_search.parent = node;
                dest_search.distance = node_search.distance + edge.distance;

                if (h.has_key(edge.dest)) {
                    h.decrease_key(edge.dest, dest_search.distance);
                } else {
                    h.insert(edge.dest, dest_search.distance);
                }
            }
        }
    }
}

list<int> LeastDistanceGraph::get_path(int start, int end) {
    dijkstra(start);
    
    list<int> path;
    if (nodes[end].search.distance == INF)
        return path;

    path.push_front(end);

    int current = end;
    while ((current = nodes[current].search.parent) != -1) {
        path.push_front(current);
    }

    return path;
}
