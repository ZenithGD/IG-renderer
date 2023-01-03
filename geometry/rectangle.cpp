#include "rectangle.hpp"

#include <core/core.hpp>

Rectangle::Rectangle(double w, double h, 
    const Vector3& o, const shared_ptr<BRDF>& brdf) 
    : Primitive(brdf), 
      x0(o.x - w / 2), z0(o.z - h / 2), x1(o.x + w / 2), z1(o.z + h / 2), k(o.y) 
    {}

Intersection Rectangle::intersection(const Ray& r, double minT, double maxT) {
    Intersection inter;

    //Solve for t
    double t = (k - r.origin.y) / r.direction.y;

    // Verify if the plane not intersect behind of the r.origin
    if (t < minT + INTERSECTION_TOLERANCE || t > maxT) {
        inter.intersects = false;
    }
    else {
        Vector3 ip = r(t);
        inter.intersects = ip.x > x0 && ip.x < x1 && ip.z > z0 && ip.z < z1;
    }

    if ( inter.intersects ) {
        
        inter.brdf = brdf;
        inter.intersections.emplace(t, Vector3(0, -1, 0));
        auto [ u, v ] = getUVCoords(r(t));

        inter.u = u; 
        inter.v = v;
    }
    return inter;
}

Vector2 Rectangle::getUVCoords(const Vector3& p) {
    return Vector2((p.x-x0)/(x1-x0), (p.z-z0)/(z1-z0));
}