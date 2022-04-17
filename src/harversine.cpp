#include "harversine.h"

#include <cmath>

using namespace std;

namespace harversine {

    double distance_in_m(pair<double, double> point1, pair<double, double> point2) {
        auto [lat1, lon1] = point1;
        auto [lat2, lon2] = point2;

        // convert to radians
        lat1 *= M_PI / 180.0;
        lat2 *= M_PI / 180.0;
        lon1 *= M_PI / 180.0;
        lon2 *= M_PI / 180.0;

        double dLat = lat2 - lat1;
        double dLon = lon2 - lon1;

        // apply formulae
        double a = pow(sin(dLat / 2), 2) + pow(sin(dLon / 2), 2) * cos(lat1) * cos(lat2);
        double rad = 6371000;
        double c = 2 * asin(sqrt(a));
        return rad * c;
    }

    double distance_in_km(pair<double, double> point1, pair<double, double> point2) {
        return distance_in_m(point1, point2) / 1000.0;
    }
}