#pragma once

#include <iostream>

#include <math/vector3.hpp>
#include <geometry/ray.hpp>
#include <geometry/primitive.hpp>

using namespace std;

/**
 * @brief Class for representing Spheres
 * 
 */
class Sphere : public Primitive {
public:

    Vector3 center;
    double radius;

    /**
     * @brief Construct a new Sphere with values
     * @param c The value of the center of the sphere
     * @param r The value of the radius of the sphere
     */
    Sphere(Vector3 c, double r, const shared_ptr<BRDF>& brdf) : Primitive(brdf), center(c), radius(r) {}

    /**
    * @brief Function of intersection with a Sphere
    * @param p The value of the Sphere that we want to prove if it intersect with the plane
    * @return SphereIntersection
    * 
    */
    Intersection intersection(const Ray& r, double minT, double maxT) override;
private:
    tuple<double, double> getUVCoords(const Vector3& point) const;
};

