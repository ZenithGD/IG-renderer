#include "rectangle.hpp"

#include <core/core.hpp>

RectangleXZ::RectangleXZ(double w, double h, 
    const Vector3& o, const shared_ptr<BRDF>& brdf) 
    : Primitive(brdf), 
      x0(o.x - w / 2), z0(o.z - h / 2), x1(o.x + w / 2), z1(o.z + h / 2), k(o.y) 
    {}

Intersection RectangleXZ::intersection(const Ray& r, double minT, double maxT) {
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
        inter.intersections.emplace(t, setFaceNormal(r.direction, Vector3(0, 1, 0)));
        auto [ u, v ] = getUVCoords(r(t));

        inter.u = u; 
        inter.v = v;
    }
    return inter;
}

Vector2 RectangleXZ::getUVCoords(const Vector3& p) {
    return Vector2((p.x-x0)/(x1-x0), (p.z-z0)/(z1-z0));
}

RectangleXY::RectangleXY(double w, double h, 
    const Vector3& o, const shared_ptr<BRDF>& brdf) 
    : Primitive(brdf), 
      x0(o.x - w / 2), y0(o.y - h / 2), x1(o.x + w / 2), y1(o.y + h / 2), k(o.z) 
    {}

Intersection RectangleXY::intersection(const Ray& r, double minT, double maxT) {
    Intersection inter;

    //Solve for t
    double t = (k - r.origin.z) / r.direction.z;

    // Verify if the plane not intersect behind of the r.origin
    if (t < minT + INTERSECTION_TOLERANCE || t > maxT) {
        inter.intersects = false;
    }
    else {
        Vector3 ip = r(t);
        inter.intersects = ip.x > x0 && ip.x < x1 && ip.y > y0 && ip.y < y1;
    }

    if ( inter.intersects ) {
        
        inter.brdf = brdf;
        inter.intersections.emplace(t, setFaceNormal(r.direction, Vector3(0, 0, 1)));
        auto [ u, v ] = getUVCoords(r(t));

        inter.u = u; 
        inter.v = v;
    }
    return inter;
}

Vector2 RectangleXY::getUVCoords(const Vector3& p) {
    return Vector2((p.x-x0)/(x1-x0), (p.y-y0)/(y1-y0));
}

RectangleYZ::RectangleYZ(double w, double h, 
    const Vector3& o, const shared_ptr<BRDF>& brdf) 
    : Primitive(brdf), 
      y0(o.y - w / 2), z0(o.z - h / 2), y1(o.y + w / 2), z1(o.z + h / 2), k(o.x) 
    {}

Intersection RectangleYZ::intersection(const Ray& r, double minT, double maxT) {
    Intersection inter;

    //Solve for t
    double t = (k - r.origin.x) / r.direction.x;

    // Verify if the plane not intersect behind of the r.origin
    if (t < minT + INTERSECTION_TOLERANCE || t > maxT) {
        inter.intersects = false;
    }
    else {
        Vector3 ip = r(t);
        inter.intersects = ip.y > y0 && ip.y < y1 && ip.z > z0 && ip.z < z1;
    }

    if ( inter.intersects ) {
        
        inter.brdf = brdf;
        inter.intersections.emplace(t, setFaceNormal(r.direction, Vector3(1, 0, 0)));
        auto [ u, v ] = getUVCoords(r(t));

        inter.u = u; 
        inter.v = v;
    }
    return inter;
}

Vector2 RectangleYZ::getUVCoords(const Vector3& p) {
    return Vector2((p.y-y0)/(y1-y0), (p.z-z0)/(z1-z0));
}