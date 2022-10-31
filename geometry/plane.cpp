#include <geometry/plane.hpp>

Intersection Plane::intersection(const Ray& r) {
    Intersection inter;

    //Solve for t
    inter.t = -((c + dot(r.origin, normal)) / dot(r.direction, normal));
 
    // Compute intersection point
    inter.point = r.origin + r.direction * inter.t; 

    // Get normal
    inter.normal = normal;

    // Verify if the plane not intersect behind of the r.origin
    if (inter.t < 0) {
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