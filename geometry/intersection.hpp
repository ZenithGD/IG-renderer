#pragma once

#include <map>
#include <memory>

#include <math/vector3.hpp>
#include <color/color.hpp>
#include <material/BRDF.hpp>

// Forward declaration of BRDF because of circular includes.
class BRDF;

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

    // BRDF of closest object
    shared_ptr<BRDF> brdf;

    // intersection UV coordinates.
    // There's only need to store the first intersection, since the albedo of a texture
    // only contributes to the BRDF and not to the BTDF.
    double u, v;

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