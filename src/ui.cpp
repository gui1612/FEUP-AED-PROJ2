#include "ui.h"

#include <iostream>
#include <string>
#include <utility>
#include <list>
#include <set>

using namespace std;

UI::UI(Pathfinding &pathfinding) : pathfinding(pathfinding) {} 

void UI::start() {

    Menu starting_point = get_starting_point_menu();
    Menu ending_point = get_ending_point_menu();
    Menu night_line = get_night_line_menu();
    Menu walk = get_toggle_walking_menu();
    Menu trip_type = get_trip_type_menu();

    Menu* menus[5] = { &starting_point, &ending_point, &night_line, &walk, &trip_type };
        
    bool is_running = true;
    while (is_running) {
        this->current_pathfinding = this->pathfinding;

        bool is_reset = false;

        MenuBlock special_block;    
        special_block.add_option("Reset", [&is_reset]() { is_reset = true; });
        special_block.add_option("Exit", [&is_running]() { is_running = false; });

        for (int i = 0; i < sizeof(menus) / sizeof(Menu*); i++) {
            Menu &menu = **(menus + i);
            
            menu.set_special_block(special_block);
            menu.show();

            if (!is_running || is_reset)
                break;
        }

        cout << endl;
        wait_for_input();
    }
}

Menu UI::get_starting_point_menu() {
    Menu starting_point("Please specify the starting point!");

    MenuBlock starting_point_options;
    starting_point_options.add_option("Coordinates", [this]() {
        pair<double, double> coordinates = this->ask_coordinates();
        this->current_pathfinding.add_bus_stop("START", "Starting point", "unknown", coordinates);
        this->start_stop = "START";
    });

    starting_point_options.add_option("Bus Stop", [this]() {
        string stop = this->ask_stop();
        this->start_stop = stop;
    });
    
    starting_point.add_block(starting_point_options);
    return starting_point;
}

Menu UI::get_ending_point_menu() {
    Menu end_point("Please specify the end point!");

    MenuBlock ending_point_options;
    ending_point_options.add_option("Coordinates", [this]() {
        pair<double, double> coordinates = this->ask_coordinates();
        this->current_pathfinding.add_bus_stop("END", "Ending point", "unknown", coordinates);
        this->end_stop = "END";
    });

    ending_point_options.add_option("Bus Stop", [this]() {
        string stop = this->ask_stop();
        this->end_stop = stop;
    });

    end_point.add_block(ending_point_options);
    return end_point;
}

Menu UI::get_toggle_walking_menu() {
    Menu walk("Are you willing to walk?");

    MenuBlock walk_options;
    walk_options.add_option("Yes", [this]() {
        double distance = this->ask_distance_on_foot();
        this->distance = distance;
    });
    
    walk_options.add_option("No", [this](){ this->distance = 0; });

    walk.add_block(walk_options);
    return walk;
}

Menu UI::get_night_line_menu() {
    Menu night_line("What part of the day is it?");

    MenuBlock night_line_options;
    
    night_line_options.add_option("[Night] 0am ~ 5am", [this]() {
        this->is_night = true;
    });

    night_line_options.add_option("[Day] 5am ~ 12pm", [this]() {
        this->is_night = false;
    });

    night_line.add_block(night_line_options);
    return night_line;
}


void UI::show_trip(const list<Pathfinding::Node> &nodes) const {
    if (nodes.size() == 0) {
        cout << "No path has been found between " << this->start_stop << " and " << this->end_stop << '!' << endl;
    } else if (nodes.size() == 1) {
        cout << "Your trip starting point is the same as your trip ending point!" << endl;
    } else {
        auto current = nodes.begin();
        auto next = nodes.begin(); next++;

        set<string> lines;
        string start_seq = current->stop;
        string end_seq = current->stop;

        if (current->lines.has_value())
            for (const auto &line : current->lines.value())
                lines.insert(line);

        while (next != nodes.end()) {
            end_seq = next->stop;

            bool contains_all = true;
            if (next->lines.has_value())
                for (const auto &line : next->lines.value()) {
                    if (!lines.contains(line)) {
                        contains_all = false;
                        break;
                    }
                }

            if (!contains_all) {
                cout << "Go from " << start_seq << " to " << end_seq << " using:" << endl;
                for (const auto &line : lines) {
                    cout << "  " << line << endl;
                }

                cout << endl;

                start_seq = next->stop;
                lines.clear();

                if (next->lines.has_value())
                    for (const auto &line : next->lines.value())
                        lines.insert(line);
            }

            current++; next++;
        }

        cout << "Go from " << start_seq << " to " << end_seq << " using:" << endl;
        for (const auto &line : lines) {
            cout << "  " << line << endl;
        }

        cout << endl;

    }
}

Menu UI::get_trip_type_menu() {
    Menu trip_type("Please select a trip type!");

    MenuBlock trip_type_options;
    trip_type_options.add_option("Least bus changes", [this](){
        if (this->distance != 0)
            this->current_pathfinding.add_on_foot_segments(distance);

        auto nodes = this->current_pathfinding.get_least_bus_changes_path(this->start_stop, this->end_stop, this->is_night);
        this->show_trip(nodes);
    });

    trip_type_options.add_option("Least distance", [this](){
        if (this->distance != 0)
            this->current_pathfinding.add_on_foot_segments(distance);

        auto nodes = this->current_pathfinding.get_least_distance_path(this->start_stop, this->end_stop, this->is_night);
        this->show_trip(nodes);
    });

    trip_type_options.add_option("Least money spent", [this](){
        if (this->distance != 0)
            this->current_pathfinding.add_on_foot_segments(distance);

        auto nodes = this->current_pathfinding.get_least_money_spent_path(this->start_stop, this->end_stop, this->is_night);
        this->show_trip(nodes);
    });

    trip_type_options.add_option("Least stops covered", [this](){
        if (this->distance != 0)
            this->current_pathfinding.add_on_foot_segments(distance);

        auto nodes = this->current_pathfinding.get_least_stops_path(this->start_stop, this->end_stop, this->is_night);
        this->show_trip(nodes);
    });

    trip_type.add_block(trip_type_options);
    return trip_type;
}

pair<double, double> UI::ask_coordinates() {
    double lat = read_value<double>("Latitude: ", "Please insert a valid latitude", [](const double &lat) { return -90 <= lat && lat <= 90; });
    double lon = read_value<double>("Longitude: ", "Please insert a valid longitude", [](const double &lon) { return -180 <= lon && lon <= 180; }); 

    return { lat, lon };
}

string UI::ask_stop() {
    string stop = read_value<string>("Stop: ", "Please insert a valid bus stop code", 
        [this](const string &stop) { return this->pathfinding.has_bus_stop(stop); }
    );

    return stop;
}

double UI::ask_distance_on_foot() {
    double distance = read_value<double>("Distance (in meters): ", "Please provide a valid distance", [](const double &distance) { return distance >= 0; });
    return distance;
}

