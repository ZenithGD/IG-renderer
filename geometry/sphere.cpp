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
    double root = (-halfb - sq) / a;
    
    if ( root < minT || maxT < root ) {
        // farthest root, ray origin inside of sphere
        root = (-halfb + sq) / a;
        if ( root < minT || maxT < root )
            return inter;
    }

    inter.intersects = true;
    inter.emission = emission;
    inter.t = root;
    inter.point = r(inter.t);
    inter.normal = normalize(inter.point - center);

    return inter;
}