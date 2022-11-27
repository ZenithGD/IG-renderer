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
    Triangle(Vector3 _pointA, Vector3 _pointB, Vector3 _pointC, BRDF brdf = BRDF())
        : Primitive(brdf), pointA(_pointA), pointB(_pointB), pointC(_pointC), 
          normal(cross(pointB - pointA, pointC - pointA)), c(-dot(pointA, normal)) {}

    /**
     * @brief Function of intersection with a Ray
     * 
     * @param p The value of the Ray that we want to prove if it intersect with the triangle
     * @return PlaneIntersection 
     */
    Intersection intersection(const Ray& r, double minT, double maxT) override;

private:
    double c;
    Vector3 normal;
    bool insideOutsideTest(Vector3 point) const;
};