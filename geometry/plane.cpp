#include <geometry/plane.hpp>

#include <core/core.hpp>

Intersection Plane::intersection(const Ray& r, double minT, double maxT) {
    Intersection inter;

    //Solve for t
    double t = -((c + dot(r.origin, normal)) / dot(r.direction, normal));

    // Verify if the plane not intersect behind of the r.origin
    if (t < minT + INTERSECTION_TOLERANCE || t > maxT) {
        inter.intersects = false;
    }
    else {
        inter.intersects = true;
    }

    if ( inter.intersects ) {
        inter.bsdf = bsdf;
        inter.intersections.emplace(t, normalize(normal));
    }
    return inter;
}