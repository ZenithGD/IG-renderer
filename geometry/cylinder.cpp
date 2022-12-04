#include <geometry/cylinder.hpp>

#include <math/coordinate.hpp>

Intersection Cylinder::intersection(const Ray& ray, double minT, double maxT) {

    Coordinate world2Obj(_perp1, _perp2, axis);

    Intersection inter = {
        .intersects = false
    };

    double a = ray.direction.x * ray.direction.x + ray.direction.y * ray.direction.y;
    double b = 2 * (ray.direction.x * ray.origin.x + ray.direction.y * ray.origin.y);
    double c = ray.origin.x * ray.origin.x + ray.origin.y * ray.origin.y - radius;

    double discr = b * b - 4 * a * c;
    if (discr < 0) return inter;

    double sq = sqrt(discr);
    // closest root
    double root1 = (-b - sq) / a;
    double root2 = (-b + sq) / a;
    
    if ( root1 < minT || maxT < root1 ) {
        // farthest root, ray origin inside of cylinder
        if ( root2 < minT || maxT < root2 )
            // no intersections
            return inter;
        else {
            inter.intersections.emplace(root2, axis);
        }
    } else {
        inter.intersections.emplace(root1, axis);
        inter.intersections.emplace(root2, axis);
    }

    inter.intersects = true;
    inter.brdf = brdf;

    return inter;
}

tuple<double, double> Cylinder::getUVCoords(const Vector3& point) const {
    // TODO
    return make_tuple(0,0);
}