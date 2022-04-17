#pragma once

#include <utility>

/**
 * @brief Utility functions for determining the distance between two points on Earth (approximation).
 */
namespace harversine {

    /**
     * @brief Determines the distance, in meters, between two points on Earth.
     * 
     * @param point1 A point (latitude and longitude)
     * @param point2 Another point (latitude and longitude)
     * @return The distance between the points, in meters.
     */
    double distance_in_m(std::pair<double, double> point1, std::pair<double, double> point2);

    /**
     * @brief Determines the distance, in kilometers, between two points on Earth.
     * 
     * @param point1 A point (latitude and longitude)
     * @param point2 Another point (latitude and longitude)
     * @return The distance between the points, in kilometers.
     */
    double distance_in_km(std::pair<double, double> point1, std::pair<double, double> point2);
}