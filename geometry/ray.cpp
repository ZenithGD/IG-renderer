#include "ray.hpp"
#include <cmath>

PlaneIntersection Ray::intersection(Plane p) {
    PlaneIntersection inter;

    //Solve for t
    double t = -((p.c + dot(origin, p.normal)) / dot(direction, p.normal));
 
    // Compute intersection point
    inter.point = origin + direction * t; 

    // Get normal
    inter.normal = p.normal;

    // Verify if the plane not intersect behind of the origin
    if (t < 0) {
        inter.intersect = false;
    }
    else {
        inter.intersect = true;
    }

    return inter;
}


SphereIntersection Ray::intersection(Sphere s) {

    Vector3 dif = origin - s.center;

    // Solve for t
    double a = dot(direction, direction);
    double b = dot(direction, dif) * 2;

    double c = dot(dif, dif) - s.radius * s.radius;

    // Get discriminant
    double discr = b * b - 4 * a * c;
    
    if ( discr < 0 ) {

        // No intersection
        return SphereIntersection{0};
    } else if ( discr == 0 ) {

        double sol = -b / (2 * a);

        // Tangent ray
        return SphereIntersection{1, eval(sol)};
    } else { 

        // Intersection

        // Check if ray's origin is inside the sphere
        double sol1 = (-b - sqrt(discr)) / (2 * a);
        double sol2 = (-b + sqrt(discr)) / (2 * a);

        if ( sol1 * sol2 < 0 ) {
            return SphereIntersection{1, eval(sol2)};
        }

        // Check if solution lies behind the ray 
        if ( sol1 < 0 ) {
            return SphereIntersection{0, eval(sol1)};
        } else {
            return SphereIntersection{2, eval(sol1), normalize(s.center - origin)};
        }
    }
}

PlaneIntersection Ray::intersection(Triangle p) {
    PlaneIntersection inter;

    //Solve for t
    double t = -((p.c + dot(origin, p.normal)) / dot(direction, p.normal));
 
    // Compute intersection point
    inter.point = origin + direction * t; 

    // Get normal
    inter.normal = p.normal;

    // Verify if the triangle not intersect behind of the origin
    if (t < 0) {
        inter.intersect = false;
    }
    else {
        inter.intersect = true;
    }

    // Verify if the point of intersect is on the limits of the triangle
    double w1, w2, w3, area;

    area = cross((p.pointB - p.pointA), (p.pointC - p.pointA)).modulus() / 2;

    w1 = cross((inter.point - p.pointB), (inter.point - p.pointC)).modulus() / area;
  
    w2 = cross((inter.point - p.pointC), (inter.point - p.pointA)).modulus() / area;
    
    w3 = 1 - w1 - w2;

    if(w1 < 0 || w1 > 1 || w2 < 0 || w2 > 1 ||  w3 < 0 || w3 > 1 || (w1 + w2 + w3) != 1) {
        inter.intersect = false;
    }

    return inter;
}