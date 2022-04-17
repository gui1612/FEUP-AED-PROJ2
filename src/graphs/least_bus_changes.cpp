#include "graphs/least_bus_changes.h"

#include <stdexcept>
#include <queue>
#include "min_heap.h"

using namespace std;

#define INF 1000000

LeastBusChangesGraph::LeastBusChangesGraph(int n) : nodes(n) {}

void LeastBusChangesGraph::add_edge(int src, int dest, int weight) {
    if (src < 0 || dest < 0 || src >= nodes.size()  || dest >= nodes.size())
        throw invalid_argument("src or dest out of bounds");

    nodes[src].adj.push_back({ dest, weight });
}

void LeastBusChangesGraph::dijkstra(int start) {
    for (int i = 0; i < nodes.size(); i++) {
        Node &node = nodes.at(i);

        node.search.parent = -1;
        node.search.distance = { INF, INF };
        node.search.visited = false;
    }

    nodes[start].search.distance = { 0, 0 };

    MinHeap<int, pair<int, int>> h(nodes.size(), -1);
    h.insert(start, { 0, 0 });

    while (h.get_size() > 0) {
        int node = h.remove_min();

        SearchInfo &node_search = nodes[node].search;
        node_search.visited = true;

        for (const Edge& edge : nodes[node].adj) {
            SearchInfo &dest_search = nodes[edge.dest].search;
            
            pair<int, int> new_distance = node_search.distance;
            new_distance.first += edge.weight;
            new_distance.second += 1;

            if (!dest_search.visited && new_distance < dest_search.distance) {
                dest_search.parent = node;
                dest_search.distance = new_distance;

                if (h.has_key(edge.dest)) {
                    h.decrease_key(edge.dest, dest_search.distance);
                } else {
                    h.insert(edge.dest, dest_search.distance);
                }
            }
        }
    }
}

list<int> LeastBusChangesGraph::get_path(int start, int end) {
    dijkstra(start);
    
    list<int> path;
    if (nodes[end].search.distance.first == INF)
        return path;

    path.push_front(end);

    int current = end;
    while ((current = nodes[current].search.parent) != -1) {
        path.push_front(current);
    }

    return path;
}

void LeastBusChangesGraph::resize(int n, bool force) {
    if (force) {
        nodes.resize(n);
        return;
    }

    if (n > nodes.size())
        nodes.resize(n + 16000);
}