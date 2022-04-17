#pragma once

#include <string>
#include "direction.h"

namespace names {

    /**
     * @brief Gets a name of a certain line formatted
     * 
     * @param name name of the line
     * @param dir direction of the line
     * 
     * @return std::string Line name in the format "[coordinates]line_start-line_end"
     * 
     * @note if `name` is not in a valid form returns the string "invalid"
     */
    std::string get_line_name(std::string name, Direction dir);

    /**
     * @brief Gets the bus stop name formatted
     * 
     * @param code code of the bus stop 
     * @param name name of the bus stop
     * 
     * @return std::string Bus Stop in the format "code-name"
     */
    std::string get_bus_stop_name(std::string code, std::string name);
}