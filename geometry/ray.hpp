#pragma once

#include <math/vector3.hpp>
#include <geometry/plane.hpp>
#include <geometry/sphere.hpp>

/**
 * @brief Struct for representing values of the intersection with a plane
 * 
 */
struct PlaneIntersection {
    Vector3 normal;
    Vector3 point;
    bool intersect;
};


/**
 * @brief Struct for representing values of the intersection with a shpere
 * 
 */
struct SphereIntersection {
    int intersections;
    Vector3 point;
    Vector3 normalVector;
};

/**
 * @brief Class for representing Ray
 * 
 */
class Ray {
public:

    /**
     * @brief Construct a new Ray with values
     * @param o The value of the origin of the ray
     * @param d The value of the direction of the ray
     */
    Ray(Vector3 o, Vector3 d) : origin(o), direction(d) {}
    Vector3 origin, direction;

    /**
    * @brief Function of intersection with a Plane
    * @param p The value of the Plane that we want to prove if it intersect with the plane
    */
    PlaneIntersection intersection(Plane p);

    /**
    * @brief Function of intersection with a Sphere
    * @param p The value of the Sphere that we want to prove if it intersect with the plane
    * 
    */
    SphereIntersection intersection(Sphere p);

    inline Vector3 eval(double t) const { return origin + direction * t; }

    inline Vector3 operator()(double t) const { return eval(t); }
};
