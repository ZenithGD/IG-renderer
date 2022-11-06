#pragma once

#include <map>

#include <math/vector3.hpp>
#include <color/color.hpp>

/**
 * @brief Struct that carries general intersection info.
 * 
 */
struct Intersection {

    bool intersects;
    
    /**
     * @brief Intersection point and normals. 
     * These are stored in increasing order of the point values, and the first 
     * entry assumes the intersection is from outside the solid shape, while the
     * second is from the inside, and so on.
     */
    std::map<double, Vector3, std::less<double>> intersections;

    // emission at intersected object
    RGB emission;

    inline double closest() const { return intersections.begin()->first; }
    inline Vector3 closestNormal() const { return intersections.begin()->second; }
};