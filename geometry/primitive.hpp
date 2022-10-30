#pragma once 

#include <color/color.hpp>
#include <math/vector3.hpp>
#include <geometry/ray.hpp>

struct Intersection {
    bool intersects;
    Vector3 normal;
    Vector3 point;
    double t;

    // emission at intersected object
    RGB emission;
};

class Primitive {
public:
    RGB emission;
    Primitive(RGB _e) : emission(_e) {}; 

    virtual Intersection intersection(const Ray& r) = 0; 
};
