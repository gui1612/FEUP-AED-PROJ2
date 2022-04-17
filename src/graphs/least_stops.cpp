#include "graphs/least_stops.h"

#include <stdexcept>
#include <queue>

using namespace std;

#define INF 1000000

LeastStopsGraph::LeastStopsGraph(int n) : nodes(n) {}

void LeastStopsGraph::add_edge(int src, int dest) {
    if (src < 0 || dest < 0 || src >= nodes.size()  || dest >= nodes.size())
        throw invalid_argument("src or dest out of bounds");

    nodes[src].adj.push_back({ dest });
}

void LeastStopsGraph::bfs(int start) {
    for (int i = 0; i < nodes.size(); i++) {
        nodes[i].search.visited = false;
        nodes[i].search.num_stops = INF;
        nodes[i].search.parent = -1;
    }
    
    nodes[start].search.visited = true;
    nodes[start].search.num_stops = 0;
    
    queue<int> q;
    q.push(start);

    while (!q.empty()) {
        int node = q.front(); q.pop();
        SearchInfo &node_search = nodes[node].search;

        for (const Edge &edge : nodes[node].adj) {
            SearchInfo &dest_search = nodes[edge.dest].search;

            if (!dest_search.visited) {
                q.push(edge.dest);

                dest_search.parent = node;
                dest_search.num_stops = node_search.num_stops + 1;
                dest_search.visited = true;
            }
        }
    }
}

list<int> LeastStopsGraph::get_path(int start, int end) {
    bfs(start);

    list<int> path;
    if (nodes[end].search.num_stops == INF)
        return path;

    path.push_front(end);

    int current = end;
    while ((current = nodes[current].search.parent) != -1) {
        path.push_front(current);
    }
    
    return path;
}
