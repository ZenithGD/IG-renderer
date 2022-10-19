#pragma once

#include <iostream>

#include <math/vector3.hpp>
#include <geometry/ray.hpp>

using namespace std;

/**
 * @brief Class for representing Spheres
 * 
 */
class Sphere {
public:

    /**
     * @brief Construct a new Sphere with values
     * @param c The value of the center of the sphere
     * @param r The value of the radius of the sphere
     */
    Sphere(Vector3 c, double r) : center(c), radius(r) {}

    Vector3 center;
    double radius;
};

