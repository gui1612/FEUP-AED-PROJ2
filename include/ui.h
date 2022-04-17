#pragma once

#include "interact.h"
#include "pathfinding.h"

#include <vector>
#include <string>

class UI {
    Pathfinding pathfinding;
    Pathfinding current_pathfinding;
    std::string start_stop;
    std::string end_stop;
    double distance;
    bool is_night;

    /**
     * @brief Asks user input for a coordinate
     * 
     * @return std::pair<double, double> Coordinates - latitude and longitude
     */
    std::pair<double, double> ask_coordinates();

    /**
     * @brief Asks user input for a stop
     * 
     * @return std::string The name of the stop if it exists
     */
    std::string ask_stop();

    /**
     * @brief Asks user input for the distance he is willing to walk
     * 
     * @return double distance
     */
    double ask_distance_on_foot();

    /**
     * @brief Displays the trip result
     * 
     * @param nodes 
     */
    void show_trip(const std::list<Pathfinding::Node> &nodes) const;

    /**
     * @brief Gets the starting point menu object
     * 
     * @return Menu 
     */
    Menu get_starting_point_menu();

    /**
     * @brief Gets the ending point menu object
     * 
     * @return Menu 
     */
    Menu get_ending_point_menu();

    /**
     * @brief Gets the toggle walking menu object
     * 
     * @return Menu 
     */
    Menu get_toggle_walking_menu();

    /**
     * @brief Gets the night line menu object
     * 
     * @return Menu 
     */
    Menu get_night_line_menu();

    /**
     * @brief Gets the trip type menu object
     * 
     * @return Menu
     */
    Menu get_trip_type_menu();

    public:
        /**
         * @brief Construct a new UI object
         * 
         * @param pathfinding Pathfinding instance
         */
        UI(Pathfinding &pathfinding);

        /**
         * @brief Displays the menu
         */
        void start();
};