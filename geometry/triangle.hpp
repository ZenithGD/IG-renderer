#pragma once 

#include <iostream>

#include <math/vector3.hpp>
#include <geometry/ray.hpp>

using namespace std;

/**
 * @brief Class for representing Triangle
 * 
 */
class Triangle {
public:
    double c;
    Vector3 normal;
    Vector3 pointA, pointB, pointC;

    /**
     * @brief Construct a new Triangle object
     * 
     * @param _c The value of c
     * @param _normal The value of the normal of the trianlge
     * @param _pointA The value of one point of the triangle
     * @param _pointB The value of one point of the triangle
     * @param _pointC The value of one point of the triangle
     */
    Triangle(double _c, Vector3 _normal, Vector3 _pointA, Vector3 _pointB, Vector3 _pointC): c(_c), normal(_normal), pointA(_pointA), pointB(_pointB), pointC(_pointC) {}

};