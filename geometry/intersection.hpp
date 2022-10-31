#pragma once

#include <math/vector3.hpp>
#include <color/color.hpp>

/**
 * @brief Struct that carries general intersection info.
 * 
 */
struct Intersection {
    bool intersects;
    Vector3 normal;
    Vector3 point;
    double t;

    // emission at intersected object
    RGB emission;
};