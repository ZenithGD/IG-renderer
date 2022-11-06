#include <geometry/sphere.hpp>

Intersection Sphere::intersection(const Ray& r, double minT, double maxT) {

    Vector3 dif = r.origin - center;
    Intersection inter;
    inter.intersects = false;

    // Solve for t
    double a = dot(r.direction, r.direction);
    double halfb = dot(r.direction, dif);

    double c = dot(dif, dif) - radius * radius;

    // Get discriminant
    double discr = halfb * halfb - a * c;

    if (discr < 0) return inter;

    double sq = sqrt(discr);
    // closest root
    double root1 = (-halfb - sq) / a;
    double root2 = (-halfb + sq) / a;
    
    if ( root1 < minT || maxT < root1 ) {
        // farthest root, ray origin inside of sphere
        if ( root2 < minT || maxT < root2 )
            // no intersections
            return inter;
        else {
            inter.intersections.emplace(root2, normalize(r(root2) - center));
        }
    } else {
        inter.intersections.emplace(root1, normalize(r(root1) - center));
        inter.intersections.emplace(root2, normalize(r(root2) - center));
    }

    inter.intersects = true;
    inter.emission = emission;

    return inter;
}