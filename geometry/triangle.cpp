#include <geometry/triangle.hpp>

bool Triangle::insideOutsideTest(Vector3 point) const {

    // TODO
    return true;
}

Intersection Triangle::intersection(const Ray& r, double minT, double maxT) {
    Intersection inter;

    //Solve for t
    inter.t = -(( _c + dot(r.origin, _normal)) / dot(r.direction, _normal));
 
    // Compute intersection point
    inter.point = r.eval(inter.t); 

    // Get normal
    inter.normal = _normal;

    // Verify if the triangle doesn't intersect behind of the ray's origin
    if (inter.t < minT || inter.t > maxT) {
        inter.intersects = false;
    }
    else {
        inter.intersects = insideOutsideTest(inter.point);
        inter.emission = emission;
    }

    return inter;
}