#pragma once

#include <iostream>

#include <geometry/ray.hpp>
#include <geometry/primitive.hpp>
#include <math/vector3.hpp>
#include <geometry/intersection.hpp>

using namespace std;

/**
 * @brief Class for representing Planes
 * 
 */
class Plane : public Primitive {
public:
    double c;
    Vector3 normal;
    
    Plane(Vector3 _pointA, Vector3 _pointB, Vector3 _pointC, const shared_ptr<BRDF>& brdf)
        : Primitive(brdf),
          normal(cross(_pointB - _pointA, _pointC - _pointA)), c(-dot(_pointA, normal)) {}

    /**
     * @brief Construct a new Plane with values
     * @param _c The value of c
     * @param _normal The value of the normal of the plane
     */
    Plane(double _c, Vector3 _normal, const shared_ptr<BRDF>& brdf) : Primitive(brdf), c(_c), normal(_normal) {}

    /**
    * @brief Function of intersection with a Ray
    * @param r The value of the Ray that we want to prove if it intersect with the plane
    * @return Intersection 
    */
    Intersection intersection(const Ray& r, double minT, double maxT) override;

};


