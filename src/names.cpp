#include "names.h"

#include <vector>
#include <iostream>

using namespace std;

namespace names {

    string get_line_name(string name, Direction dir) {
        size_t first_dash = name.find_first_of('-');
        size_t second_dash = name.find_first_of('-', first_dash + 1);

        if (first_dash == string::npos || second_dash == string::npos)
            return "invalid";

        string part1 = name.substr(0, first_dash);
        string part2 = name.substr(first_dash + 1, second_dash - first_dash - 1);
        string part3 = name.substr(second_dash + 1);

        size_t first_char1 = part1.find_first_not_of(' '), last_char1 = part1.find_last_not_of(' ');
        size_t first_char2 = part2.find_first_not_of(' '), last_char2 = part2.find_last_not_of(' ');
        size_t first_char3 = part3.find_first_not_of(' '), last_char3 = part3.find_last_not_of(' ');

        part1 = part1.substr(first_char1, last_char1 - first_char1 + 1);
        part2 = part2.substr(first_char2, last_char2 - first_char2 + 1);
        part3 = part3.substr(first_char3, last_char3 - first_char3 + 1);

        switch (dir) {
            case Direction::NORMAL:
                return "[" + part1 + "] " + part2 + " - " + part3;
            case Direction::REVERSED:
                return "[" + part1 + "] " + part3 + " - " + part2;
        };

        return "invalid";
    }

    std::string get_bus_stop_name(std::string code, std::string name) {
        size_t first_char1 = code.find_first_not_of(' '), last_char1 = code.find_last_not_of(' ');
        size_t first_char2 = name.find_first_not_of(' '), last_char2 = name.find_last_not_of(' ');

        code = code.substr(first_char1, last_char1 - first_char1 + 1);
        name = name.substr(first_char2, last_char2 - first_char2 + 1);

        return code + " - " + name;
    }
}