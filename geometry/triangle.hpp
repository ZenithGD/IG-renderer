#pragma once 

#include <iostream>

#include <math/vector3.hpp>
#include <math/vector2.hpp>

#include <geometry/ray.hpp>
#include <geometry/primitive.hpp>
#include <geometry/plane.hpp>

using namespace std;

struct VertexInfo {
    Vector3 pos;
    Vector3 normal;
    Vector2 uv;
};

/**
 * @brief Class for representing Triangle
 * 
 */
class Triangle : public Primitive {
public:
    VertexInfo pointA, pointB, pointC;

    /**
     * @brief Construct a new Triangle object
     * 
     * @param _pointA Triangle's first vertex
     * @param _pointB Triangle's second vertex
     * @param _pointC Triangle's third vertex
     */
    Triangle(const VertexInfo& _pointA, const VertexInfo& _pointB, const VertexInfo& _pointC, const shared_ptr<BRDF>& brdf)
        : Primitive(brdf),
          normal(cross(_pointB.pos - _pointA.pos, _pointC.pos - _pointA.pos)),
          pointA(_pointA),
          pointB(_pointB),
          pointC(_pointC),
          c(-dot(_pointA.pos, normal)),
          n2(dot(normal, normal)) {}

    /**
     * @brief Function of intersection with a Ray
     * Computes the intersection with the Möller-Trumbore method.
     * @param p The value of the Ray that we want to prove if it intersect with the triangle
     * @return PlaneIntersection 
     */
    Intersection intersection(const Ray& r, double minT, double maxT) override;

private:
    bool insideOutsideTest(Vector3 point, double& u, double& v) const;

    Vector3 normal;

    // precomputed values
    double c, n2;
};