#pragma once

#include <map>

#include <math/vector3.hpp>
#include <color/color.hpp>
#include <scene/BSDF.hpp>

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
    std::multimap<double, Vector3, std::less<double>> intersections;

    // BSDF of closest object
    BSDF bsdf;

    /**
     * @brief Return closest value of intersection
     * 
     * @return double 
     */
    inline double closest() const { return intersections.begin()->first; }

    /**
     * @brief Return normal at the closest intersection
     * 
     * @return Vector3 
     */
    inline Vector3 closestNormal() const { return intersections.begin()->second; }

    friend ostream& operator<<(ostream& os, const Intersection& it);
};

ostream& operator<<(ostream& os, const Intersection& it);