#include <geometry/triangle.hpp>

#include <core/core.hpp>

bool Triangle::insideOutsideTest(Vector3 point) const {

    return true;

}

Intersection Triangle::intersection(const Ray& r, double minT, double maxT) {
    Intersection inter;

    //Solve for t
    double t = -((c + dot(r.origin, normal)) / dot(r.direction, normal));

    // Verify if the plane not intersect behind of the r.origin
    if (t < minT + INTERSECTION_TOLERANCE || t > maxT) {
        inter.intersects = false;
    }
    else {
        inter.intersects = insideOutsideTest(r(t));
    }

    if ( inter.intersects ) {
        inter.brdf = brdf;
        inter.intersections.emplace(t, normalize(normal));
    }
    return inter;
}