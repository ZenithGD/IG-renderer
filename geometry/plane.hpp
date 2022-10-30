#pragma once

#include <iostream>

#include <math/vector3.hpp>
#include <geometry/ray.hpp>
#include <geometry/primitive.hpp>

using namespace std;

/**
 * @brief Struct for representing values of the intersection with a plane
 * 
 */
struct PlaneIntersection : public Intersection {};

/**
 * @brief Class for representing Planes
 * 
 */
class Plane : public Primitive {
public:
    double c;
    Vector3 normal;
    
    /**
     * @brief Construct a new Plane with values
     * @param _c The value of c
     * @param _normal The value of the normal of the plane
     */
    Plane(double _c, Vector3 _normal, RGB emission) : Primitive(emission), c(_c), normal(_normal) {}

    /**
    * @brief Function of intersection with a Ray
    * @param r The value of the Ray that we want to prove if it intersect with the plane
    * @return PlaneIntersection 
    */
    Intersection intersection(const Ray& r) override;

};


