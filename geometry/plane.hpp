#pragma once

#include <iostream>

#include <math/vector3.hpp>
#include <geometry/ray.hpp>

using namespace std;

/**
 * @brief Class for representing Planes
 * 
 */
class Plane {
public:
    double c;
    Vector3 normal;
    
    /**
     * @brief Construct a new Plane with values
     * @param _c The value of c
     * @param _normal The value of the normal of the plane
     */
    Plane(double _c, Vector3 _normal ): c(_c), normal(_normal) {}

};


