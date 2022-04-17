#include "dataset.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <limits>
#include <sstream>

using namespace std;

namespace dataset {

    ifstream open_file(std::string filename) {
        ifstream file;
        file.open(filename);
        if (!file) {
            throw invalid_argument("Couldn't open file");
        }

        return file;
    }
    
    Line read_line(std::string code, Direction dir) {        
        string filename = DATASET + "line_" + code + "_" + to_string(dir) + ".csv";
        ifstream file = open_file(filename);

        string number_of_stops;
        getline(file, number_of_stops);
        int num_stops = stoi(number_of_stops);

        Line line;
        line.code = code;
        line.dir = dir;

        for (int i = 0; i < num_stops; i++) {
            string stop;
            getline(file, stop);

            line.stops.push_back(stop);
        }

        file.close();

        return line;
    }

    vector<Stop> read_stops() {
        string filename = DATASET + "stops.csv";
        ifstream file = open_file(filename);

        vector<Stop> stops;

        file.ignore(numeric_limits<streamsize>::max(), '\n');
        
        string line;
        while (getline(file, line)) {
            istringstream stream(line);

            Stop stop;
            getline(stream, stop.code, ',');
            getline(stream, stop.name, ',');
            getline(stream, stop.zone, ',');
            
            string longitude, latitude;
            getline(stream, latitude, ',');
            getline(stream, longitude, '\n');
            stop.point = { stod(latitude), stod(longitude) };

            stops.push_back(stop);
        }

        return stops;
    }

    vector<Line> read_lines() {
        string filename = DATASET + "lines.csv";
        ifstream file = open_file(filename);

        vector<Line> lines;

        file.ignore(numeric_limits<streamsize>::max(), '\n');
        
        string line;
        while (getline(file, line)) {
            istringstream stream(line);

            string code, name;
            getline(stream, code, ',');
            getline(stream, name, ',');

            Line line1 = read_line(code, Direction::NORMAL);
            Line line2 = read_line(code, Direction::REVERSED);

            line1.name = name;
            line2.name = name;
            
            if (!line1.stops.empty()) lines.push_back(line1);
            if (!line2.stops.empty()) lines.push_back(line2);
        }

        return lines;
    }
}
