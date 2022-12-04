#pragma once 

#include <geometry/primitive.hpp>

class Cylinder : public Primitive {
public:
    Vector3 origin, axis;
    double radius, height;

    Cylinder(const Vector3 ax, double r, double h, const shared_ptr<BRDF>& brdf)
        : Primitive(brdf), axis(ax), radius(r), height(h), 
          _perp1(normalize(perpendicular(ax))), _perp2(normalize(cross(ax, _perp1))) {}

    Intersection intersection(const Ray& ray, double minT, double maxT) override;
private:
    tuple<double, double> getUVCoords(const Vector3& point) const;

    Vector3 _perp1, _perp2;
};