#pragma once 

#include <geometry/primitive.hpp>

class Cylinder : public Primitive {
public:
    Cylinder(const Vector3 ax, double r, double h, BSDF bsdf = BSDF())
        : Primitive(bsdf), axis(ax), radius(r), height(h), 
          _perp1(normalize(perpendicular(ax))), _perp2(normalize(cross(ax, _perp1))) {}

    Intersection intersection(const Ray& ray, double minT, double maxT) override;
        
    Vector3 origin, axis;
    double radius, height;
private:

    Vector3 _perp1, _perp2;
};