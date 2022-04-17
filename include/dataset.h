#pragma once

#include <string>
#include <vector>
#include <list>
#include <map>
#include <utility>

#include "config.h"
#include "direction.h"


namespace dataset {

    struct Line {
        std::string code;
        Direction dir;

        std::string name = "unknown";
        std::list<std::string> stops;
    };

    struct Stop {
        std::string code;

        std::string name = "unknown";
        std::string zone;
        std::pair<double, double> point;
    };

    /**
     * 
     * @brief Reads the information stored for a certain line
     *
     * @param names 
     * @param line The line's code
     * @param dir The line's direction
     * @return Returns a Line instance
     */
    Line read_line(std::map<std::string, std::string> names, std::string line, Direction dir);

    /**
     * @brief Reads the stops from the files
     * 
     * @return std::vector<Stop> Returns a vector of Stop instances
     */
    std::vector<Stop> read_stops();

    /**
     * @brief Reads the lines from the files
     * 
     * @return std::vector<Line> Returns a vector of Line instances
     */
    std::vector<Line> read_lines();
}