#include <iostream>

#include <string>
#include <sstream>
#include <utility>
#include "config.h"
#include "dataset.h"
#include "names.h"
#include "pathfinding.h"
#include "harversine.h"
#include "ui.h"

using namespace std;




int main() {

    try {
        Pathfinding path;

        for (const auto &stop : dataset::read_stops())
            path.add_bus_stop(stop.code, stop.name, stop.zone, stop.point);

        for (const auto &line : dataset::read_lines()) {
            path.add_line(line.code, line.dir, line.name);

            for (const auto &stop : line.stops)
                path.add_line_stop(line.code, line.dir, stop);
        }

        UI ui{path};
        ui.start();

    } catch (end_of_file_exception &err) {
        cerr << err.what();
    }

    return 0;
}

//     path.add_on_foot_segments(300);

//     auto l = path.get_least_bus_changes_path("HSJ12", "CQ8");
//     auto current = l.begin();
//     auto next = l.begin(); next++;

//     while (next != l.end() && current != l.end()) {
//         cout << (*current).stop << ',';
//         cout << (*next).stop << ',';
//         cout << (*next).lines.value_or<list<string>>({ "" }).front() << ',';

//         cout << (*next).lines.value_or<list<string>>({ "" }).front() << endl;
//         current++; next++;
//     }

// //     cout << "Pog1";

//     for (const auto node : l) {
//         cout << "STOP: " << node.stop << endl;
//         for (const auto line : node.lines.value_or<list<string>>({})) {
//             cout << "LINE: " << line << endl;
//         }

// // cout << "Pog2";
// //         cout << endl;
// //     }
// //     cout << "Pog3";




    // for (const auto &stop : dataset::read_stops()) {
    //     cout << "CODE: " << stop.code << '\n'
    //         << "NAME: " << stop.name << '\n'
    //         << "ZONE: " << stop.zone << '\n'
    //         << "POINT: (" << stop.point.first << ", " << stop.point.second << ')' << '\n'
    //         << endl;
    // }

    // cin.ignore(1);

    // for (const auto &line : dataset::read_lines()) {
    //     cout << "CODE: " << line.code << '\n'
    //         << "NAME: " << line.name << '\n'
    //         << "DIRECTION: " << line.dir << '\n'
    //         << "STOPS: " << '\n';

    //     for (const auto &stop : line.stops) {
    //         cout << '\t' << stop << '\n';
    //     }

    //     cout << '\n' << endl;
    // }
