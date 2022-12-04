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
        inter.brdf = brdf;
        inter.intersections.emplace(t, normalize(normal));
        auto [ u, v ] = getUVCoords(r(t));

        inter.u = u; 
        inter.v = v;
    }
    return inter;
}

tuple<double, double> Plane::getUVCoords(const Vector3& point) const {
    // UV coordinates on a plane are equal to the length of the projection
    // to the U and V parametric axes of the plane.
    return make_tuple<double, double>( dot(uAxis, point), dot(vAxis, point) );
}