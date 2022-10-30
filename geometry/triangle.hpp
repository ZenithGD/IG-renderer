#pragma once 

#include <iostream>

#include <math/vector3.hpp>
#include <geometry/ray.hpp>
#include <geometry/primitive.hpp>
#include <geometry/plane.hpp>

using namespace std;

/**
 * @brief Class for representing Triangle
 * 
 */
class Triangle : public Primitive {
public:
    Vector3 pointA, pointB, pointC;

    /**
     * @brief Construct a new Triangle object
     * 
     * @param _pointA Triangle's first vertex
     * @param _pointB Triangle's second vertex
     * @param _pointC Triangle's third vertex
     */
    Triangle(Vector3 _pointA, Vector3 _pointB, Vector3 _pointC, RGB emission)
        : Primitive(emission), pointA(_pointA), pointB(_pointB), pointC(_pointC), 
          _normal(cross(pointB - pointA, pointC - pointA)), _c(-dot(pointA, _normal)) {}

    /**
     * @brief Function of intersection with a Ray
     * 
     * @param p The value of the Ray that we want to prove if it intersect with the triangle
     * @return PlaneIntersection 
     */
    Intersection intersection(const Ray& r) override;

private:
    double _c;
    Vector3 _normal;
    bool insideOutsideTest(Vector3 point);
};