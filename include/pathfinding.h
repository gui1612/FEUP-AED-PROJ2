#pragma once

#include <string>
#include <optional>
#include <list>
#include <unordered_map>
#include <vector>
#include "direction.h"

class Pathfinding {

    /**
     * @brief Represents a Line
     */
    struct Line {
        std::string code;   // Code of the line
        Direction dir;      // Direction of the line
        std::string name;   // Name of the line

        int first_bus_stop = -1;
    };

    /**
     * @brief Represents a bus stop
     */
    struct BusStop {
        std::string code;   // Code of the stop

        std::string name;   // Name of the stop
        std::string zone;   // Zone of the stop
        std::pair<double, double> point;    // Coordinates of the stop

        std::unordered_map<std::string, int> destinations;  // Maps each possible destination to a value
    };


    std::unordered_map<std::string, int> bus_stop_positions;    // Maps each stop to a value
    std::unordered_map<std::string, int> line_positions;        // Maps each line to a value

    std::vector<Line> lines;            // Constains all the existing lines
    std::vector<BusStop> bus_stops;     // Contains all the bus existing stops

    std::string get_line_key(std::string code, Direction dir) const;

    int get_last_bus_stop(std::string code, Direction dir) const;
    std::list<int> get_bus_stops(std::string code, Direction dir) const;

    public:

        /**
         * @brief Represents a Node
         */
        struct Node {
            std::string stop;
            std::optional<std::list<std::string>> lines;
        };

        /**
         * @brief Returns a boolean that represents wether a line
         * with certain properties exists
         * 
         * @param code code of the line
         * @param dir direction of the line
         * @return true line exists
         * @return false line does not exist
         */
        bool has_line(std::string code, Direction dir) const;

        /**
         * @brief Returns a boolean that represents wether a BusStop
         * with certain properties exists
         * 
         * @param code code of the stop
         * @return true stop exists
         * @return false stop does not exist
         */
        bool has_bus_stop(std::string code) const;

        /**
         * @brief Returns a boolean that represents wether a line
         * is a night line or not
         * 
         * @param code code of the line
         * @return true line is a night line
         * @return false line is not a night line
         */
        bool is_night_line(std::string code) const;

        /**
         * @brief Adds a Line with certain properties to the Pathfinding instance 
         * 
         * @param code Code of the line
         * @param dir Direction of the line
         * @param name Name of the line
         */
        void add_line(std::string code, Direction dir, std::string name);
    
        /** 
         * @brief Adds a Stop with certain properties to the Pathfinding instance 
         * 
         * @param code Code of the stop
         * @param name Name of the line
         * @param zone Zone of the line
         * @param point Coordinates of the BusStop
         */
        void add_bus_stop(std::string code, std::string name, std::string zone, std::pair<double, double> point);

        /**
         * @brief Adds a Stop with certain properties to the Pathfinding instance 
         * 
         * @param line_code Code of the line
         * @param dir Direction of the line
         * @param bus_stop_code Code of the bus stop
         */
        void add_line_stop(std::string line_code, Direction dir, std::string bus_stop_code);

        /**
         * @brief Inserts new nodes in the graph according to trajectories that won't be taken on bus
         * 
         * @param max_distance The maximum distance that can be walked
         */
        void add_on_foot_segments(double max_distance);

        /**
         * @brief Calculates the path with the least distance
         * 
         * @param start Starting Stop
         * @param end Destination Stop
         * @param is_night Boolean value that represents wether it is a night
         * @return std::list<Node> List of Stops and Lines traversed
         */
        std::list<Node> get_least_distance_path(std::string start, std::string end, bool is_night);

        /**
         * @brief Calculates the path with the least number of stops
         * 
         * @param start Starting Stop
         * @param end Destination Stop
         * @param is_night Boolean value that represents wether it is a night
         * @return std::list<Node> List of Stops and Lines traversed
         */
        std::list<Node> get_least_stops_path(std::string start, std::string end, bool is_night);

        /**
         * @brief Calculates the path with the least money spent
         * 
         * @param start Starting Stop
         * @param end Destination Stop
         * @param is_night Boolean value that represents wether it is a night
         * @return std::list<Node> List of Stops and Lines traversed
         */
        std::list<Node> get_least_money_spent_path(std::string start, std::string end, bool is_night);
        
        /**
         * @brief Calculates the path with the least bus changes
         * 
         * @param start Starting Stop
         * @param end Destination Stop
         * @param is_night Boolean value that represents wether it is a night
         * @return std::list<Node> List of Stops and Lines traversed
         */
        std::list<Node> get_least_bus_changes_path(std::string start, std::string end, bool is_night);
};

