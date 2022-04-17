#include "pathfinding.h"

#include <stdexcept>
#include <iostream>
#include <deque>

#include "graphs/least_distance.h"
#include "graphs/least_stops.h"
#include "graphs/least_money_spent.h"
#include "graphs/least_bus_changes.h"
#include "harversine.h"
#include "names.h"

using namespace std;

bool Pathfinding::has_line(string code, Direction dir) const {
    return line_positions.contains(get_line_key(code, dir));
}

bool Pathfinding::has_bus_stop(string code) const {
    return bus_stop_positions.contains(code);
}

bool Pathfinding::is_night_line(string code) const {
    return code.at(code.size() - 1) == 'M';
}

string Pathfinding::get_line_key(string code, Direction dir) const {
    return code + '/' + to_string(dir);
}

list<int> Pathfinding::get_bus_stops(string code, Direction dir) const {
    if (!has_line(code, dir)) return {};

    string line_key = get_line_key(code, dir);
    int current = lines.at(line_positions.at(line_key)).first_bus_stop;
    if (current == -1) return {};

    list<int> stops;
    while ((stops.empty() || stops.front() != current) && bus_stops.at(current).destinations.contains(line_key)) {
        stops.push_back(current);
        current = bus_stops.at(current).destinations.at(line_key);
    }

    stops.push_back(current);
    return stops;
}

int Pathfinding::get_last_bus_stop(string code, Direction dir) const {
    return get_bus_stops(code, dir).back();
}

void Pathfinding::add_line(string code, Direction dir, string name) {
    if (has_line(code, dir)) return;

    lines.push_back({ code, dir, name });
    line_positions.insert({ get_line_key(code, dir), lines.size() - 1 });
}

void Pathfinding::add_bus_stop(string code, string name, string zone, pair<double, double> point) {
    if (has_bus_stop(code)) return;

    bus_stops.push_back({ code, name, zone, point });
    bus_stop_positions.insert({ code, bus_stops.size() - 1 });
}

void Pathfinding::add_line_stop(string line_code, Direction dir, string bus_stop_code) {
    if (!has_line(line_code, dir) || !has_bus_stop(bus_stop_code)) return;

    int line_index = line_positions.at(get_line_key(line_code, dir));
    Line &line = lines.at(line_index);

    int bus_stop_index = bus_stop_positions.at(bus_stop_code);
    if (line.first_bus_stop == -1) {
        line.first_bus_stop = bus_stop_index;
        return;
    }

    int last_stop = get_last_bus_stop(line_code, dir);
    bus_stops.at(last_stop).destinations.insert({ get_line_key(line_code, dir), bus_stop_index });
}

void Pathfinding::add_on_foot_segments(double max_distance) {
    for (int i = 0; i < bus_stops.size(); i++) {
        for (int j = i + 1; j < bus_stops.size(); j++) {
            BusStop &stop1 = bus_stops[i];
            BusStop &stop2 = bus_stops[j];
            
            double distance = harversine::distance_in_m(stop1.point, stop2.point);
            if (distance > max_distance) continue;

            string line_code = stop1.code + " - " + stop2.code;
            if (!has_line(line_code, Direction::NORMAL)) {
                add_line(line_code, Direction::NORMAL, "On foot - " + stop1.code + " - " + stop2.code);
                add_line_stop(line_code, Direction::NORMAL, stop1.code);
                add_line_stop(line_code, Direction::NORMAL, stop2.code);
            }

            if (!has_line(line_code, Direction::REVERSED)) {
                add_line(line_code, Direction::REVERSED, "On foot - " + stop1.code + " - " + stop2.code);
                add_line_stop(line_code, Direction::REVERSED, stop2.code);
                add_line_stop(line_code, Direction::REVERSED, stop1.code);
            }
        }
    }
}

list<Pathfinding::Node> Pathfinding::get_least_distance_path(string start, string end, bool is_night) {
    if (!has_bus_stop(start) || !has_bus_stop(end)) return {};

    LeastDistanceGraph graph(bus_stops.size());

    for (const Line &line : lines) {
        if (is_night != is_night_line(line.code)) continue;

        list<int> line_bus_stops = get_bus_stops(line.code, line.dir);
        
        auto current = line_bus_stops.begin();
        auto next = line_bus_stops.begin(); next++;

        while (current != line_bus_stops.end() && next != line_bus_stops.end()) {
            graph.add_edge(
                *current,
                *next,
                harversine::distance_in_m(
                    bus_stops.at(*current).point,
                    bus_stops.at(*next).point
                )
            );

            current++; next++;
        }
    }

    list<Pathfinding::Node> result;
    
    list<int> path = graph.get_path(bus_stop_positions.at(start), bus_stop_positions.at(end));

    auto current = path.begin();
    auto next = path.begin(); next++;

    while (current != path.end()) {
        BusStop &stop = bus_stops.at(*current);

        Pathfinding::Node node;
        node.stop = stop.code + " - " + stop.name;

        if (next != path.end()) {
            list<string> connecting_lines;

            auto it = stop.destinations.begin();
            while (it != stop.destinations.end()) {
                if ((*it).second == *next) {
                    int line_index = line_positions.at((*it).first);
                    Line &line = lines.at(line_index);
                    if (is_night != is_night_line(line.code)) {
                        it++;
                        continue;
                    }

                    connecting_lines.push_back(names::get_line_name(line.name, line.dir));
                }

                it++;
            }

            node.lines = connecting_lines;
        }

        result.push_back(node);
        current++; next++;
    }
    
    return result;
}

list<Pathfinding::Node> Pathfinding::get_least_stops_path(string start, string end, bool is_night) {
    if (!has_bus_stop(start) || !has_bus_stop(end)) return {};

    LeastStopsGraph graph(bus_stops.size());

    for (const Line &line : lines) {
        if (is_night != is_night_line(line.code)) continue;

        list<int> line_bus_stops = get_bus_stops(line.code, line.dir);
        
        auto current = line_bus_stops.begin();
        auto next = line_bus_stops.begin(); next++;

        while (current != line_bus_stops.end() && next != line_bus_stops.end()) {
            graph.add_edge(
                *current,
                *next
            );

            current++; next++;
        }
    }

    list<Pathfinding::Node> result;
    
    list<int> path = graph.get_path(bus_stop_positions.at(start), bus_stop_positions.at(end));

    auto current = path.begin();
    auto next = path.begin(); next++;

    while (current != path.end()) {
        BusStop &stop = bus_stops.at(*current);

        Pathfinding::Node node;
        node.stop = stop.code + " - " + stop.name;

        if (next != path.end()) {
            list<string> connecting_lines;

            auto it = stop.destinations.begin();
            while (it != stop.destinations.end()) {
                if ((*it).second == *next) {
                    int line_index = line_positions.at((*it).first);
                    Line &line = lines.at(line_index);
                    if (is_night != is_night_line(line.code)) {
                        it++;
                        continue;
                    }

                    connecting_lines.push_back(names::get_line_name(line.name, line.dir));
                }

                it++;
            }

            node.lines = connecting_lines;
        }

        result.push_back(node);
        current++; next++;
    }
    
    return result;
}

list<Pathfinding::Node> Pathfinding::get_least_money_spent_path(string start, string end, bool is_night) {
    if (!has_bus_stop(start) || !has_bus_stop(end)) return {};

    vector<string> zones(bus_stops.size());
    for (int i = 0; i < bus_stops.size(); i++) {
        zones.at(i) = bus_stops[i].zone;
    }

    LeastMoneySpentGraph graph(bus_stops.size(), zones);

    for (const Line &line : lines) {
        if (is_night != is_night_line(line.code)) continue;

        list<int> line_bus_stops = get_bus_stops(line.code, line.dir);
        
        auto current = line_bus_stops.begin();
        auto next = line_bus_stops.begin(); next++;

        while (current != line_bus_stops.end() && next != line_bus_stops.end()) {
            graph.add_edge(
                *current,
                *next
            );

            current++; next++;
        }
    }

    list<Pathfinding::Node> result;
    
    list<int> path = graph.get_path(bus_stop_positions.at(start), bus_stop_positions.at(end));

    auto current = path.begin();
    auto next = path.begin(); next++;

    while (current != path.end()) {
        BusStop &stop = bus_stops.at(*current);

        Pathfinding::Node node;
        node.stop = stop.code + " - " + stop.name;

        if (next != path.end()) {
            list<string> connecting_lines;

            auto it = stop.destinations.begin();
            while (it != stop.destinations.end()) {
                if ((*it).second == *next) {
                    int line_index = line_positions.at((*it).first);
                    Line &line = lines.at(line_index);
                    if (is_night != is_night_line(line.code)) {
                        it++;
                        continue;
                    }

                    connecting_lines.push_back(names::get_line_name(line.name, line.dir));
                }

                it++;
            }

            node.lines = connecting_lines;
        }

        result.push_back(node);
        current++; next++;
    }
    
    return result;
}

list<Pathfinding::Node> Pathfinding::get_least_bus_changes_path(string start, string end, bool is_night) {
    if (!has_bus_stop(start) || !has_bus_stop(end)) return {};

    deque<pair<int, int>> cluster_nodes;
    vector<vector<int>> nodes_by_stop(bus_stops.size());

    LeastBusChangesGraph graph(1800000);

    for (int i = 0; i < lines.size(); i++) {
        if (is_night != is_night_line(lines[i].code)) continue;

        list<int> stops = get_bus_stops(lines[i].code, lines[i].dir);
        int num_stops = stops.front() == stops.back() ? stops.size() - 1 : stops.size();

        int start = cluster_nodes.size();
        auto it = stops.begin();
        for (int j = 0; j < num_stops; j++) {
            cluster_nodes.push_back({ *it, i });
            nodes_by_stop[*it].push_back(cluster_nodes.size() - 1);

            it++;
        }
            
        graph.resize(cluster_nodes.size(), false);
        for (int j = start; j < cluster_nodes.size() - 1; j++) {
            graph.add_edge(j, j + 1, 0);
        }
    }

    for (int i = 0; i < nodes_by_stop.size(); i++) {
        cluster_nodes.push_back({ i, -2 });
        int index = cluster_nodes.size() - 1;
        graph.resize(cluster_nodes.size(), false);

        for (const auto &stop : nodes_by_stop[i]) {
            graph.add_edge(stop, index, 1);
            graph.add_edge(index, stop, 0);
        }
    }

    int start_stop_index = bus_stop_positions.at(start);
    int end_stop_index = bus_stop_positions.at(end);

    cluster_nodes.push_back({ start_stop_index, -1 });
    int start_node = cluster_nodes.size() - 1;

    cluster_nodes.push_back({ end_stop_index, -1 });
    int end_node = cluster_nodes.size() - 1;

    graph.resize(cluster_nodes.size(), true);
    for (const auto &node : nodes_by_stop[start_stop_index]) {
        graph.add_edge(start_node, node, 0);
    }

    for (const auto &node : nodes_by_stop[end_stop_index]) {
        graph.add_edge(node, end_node, 0);
    }

    list<int> path = graph.get_path(start_node, end_node);
    if (path.size() < 3) return {};

    list<Pathfinding::Node> result;    

    path.pop_front();
    path.pop_back();

    for (const auto p : path) {
        int bus_stop_index = cluster_nodes[p].first;
        int line_index = cluster_nodes[p].second;
        if (line_index < 0) continue;

        Pathfinding::Node node;
        node.stop = bus_stops[bus_stop_index].code + " - " + bus_stops[bus_stop_index].name;
        node.lines = { names::get_line_name(lines[line_index].name, lines[line_index].dir) };

        if (!result.empty() && result.back().stop == node.stop)
            result.back() = node;
        else
            result.push_back(node);
    }

    result.back().lines = nullopt;
    
    return result;
}