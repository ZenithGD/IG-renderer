#include <geometry/sphere.hpp>

Intersection Sphere::intersection(const Ray& r) {

    Vector3 dif = r.origin - center;
    SphereIntersection inter;

    // Solve for t
    double a = dot(r.direction, r.direction);
    double b = dot(r.direction, dif) * 2;

    double c = dot(dif, dif) - radius * radius;

    // Get discriminant
    double discr = b * b - 4 * a * c;
    
    if ( discr < 0 ) {

        // No intersection
        inter.intersections = 0;

    } else if ( discr == 0 ) {

        double sol = -b / (2 * a);

        // Tangent ray
        inter.intersections = 1;
        inter.point = r.eval(sol);
        inter.t = sol;
    } else { 

        // Intersection

        // Check if ray's r.origin is inside the sphere
        double sol1 = (-b - sqrt(discr)) / (2 * a);
        double sol2 = (-b + sqrt(discr)) / (2 * a);

        if ( sol1 * sol2 < 0 ) {
            inter.intersections = 1;
            inter.point = r.eval(sol2);
            inter.t = sol2;
        } else {
            inter.t = sol1;
            // Check if solution lies behind the ray 
            if ( sol1 < 0 ) {
                inter.intersections = 0;
                inter.point = r.eval(sol1);
            } else {
                inter.intersections = 2;
                inter.point = r.eval(sol1);
                inter.normal = normalize(center - inter.point);
            }
        }
    }

    inter.intersects = inter.intersections > 0;

    if ( inter.intersects ) {
        inter.emission = emission;
    }
    return inter;
}