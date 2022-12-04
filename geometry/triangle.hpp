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
    Vector3 pointA, pointB, pointC, uAxis, vAxis;

    /**
     * @brief Construct a new Triangle object
     * 
     * @param _pointA Triangle's first vertex
     * @param _pointB Triangle's second vertex
     * @param _pointC Triangle's third vertex
     */
    Triangle(const Vector3& _pointA, const Vector3& _pointB, const Vector3& _pointC, const shared_ptr<BRDF>& brdf)
        : Primitive(brdf),
          normal(cross(_pointB - _pointA, _pointC - _pointB)),
          c(-dot(_pointA, normal)),
          pointA(_pointA),
          pointB(_pointB),
          pointC(_pointC),
          uAxis(_pointB - _pointA),
          vAxis(_pointC - _pointA) {}

    /**
     * @brief Function of intersection with a Ray
     * 
     * @param p The value of the Ray that we want to prove if it intersect with the triangle
     * @return PlaneIntersection 
     */
    Intersection intersection(const Ray& r, double minT, double maxT) override;
private:

    tuple<double, double> getUVCoords(const Vector3& point) const;
    
    double c;
    Vector3 normal;
    bool insideOutsideTest(Vector3 point) const;
};