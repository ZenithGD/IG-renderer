#include <geometry/plane.hpp>

Intersection Plane::intersection(const Ray& r, double minT, double maxT) {
    Intersection inter;

    //Solve for t
    inter.t = -((c + dot(r.origin, normal)) / dot(r.direction, normal));
 
    // Compute intersection point
    inter.point = r.origin + r.direction * inter.t; 

    // Get normal
    inter.normal = normal;

    // Verify if the plane not intersect behind of the r.origin
    if (inter.t < minT || inter.t > maxT) {
        inter.intersects = false;
    }
    else {
        inter.intersects = true;
    }

    if ( inter.intersects ) {
        inter.emission = emission;
    }
    return inter;
}