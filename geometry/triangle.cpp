#include <geometry/triangle.hpp>

Intersection Triangle::intersection(const Ray& r){
    PlaneIntersection inter;

    //Solve for t
    inter.t = -(( c + dot(r.origin,  normal)) / dot(r.direction,  normal));
 
    // Compute intersection point
    inter.point = r.origin + r.direction * inter.t; 

    // Get normal
    inter.normal =  normal;

    // Verify if the triangle not intersect behind of the r.origin
    if (inter.t < 0) {
        inter.intersects = false;
    }
    else {
        inter.intersects = true;
    }

    // Verify if the point of intersect is on the limits of the triangle
    double w1, w2, w3, area;

    area = cross(( pointB - pointA),  (pointC - pointA)).modulus() / 2;

    w1 = cross((inter.point - pointB), (inter.point - pointC)).modulus() / area;
  
    w2 = cross((inter.point - pointC), (inter.point - pointA)).modulus() / area;
    
    w3 = 1 - w1 - w2;

    if(w1 < 0 || w1 > 1 || w2 < 0 || w2 > 1 ||  w3 < 0 || w3 > 1 || (w1 + w2 + w3) != 1) {
        inter.intersects = false;
    }

    if ( inter.intersects ) {
        inter.emission = emission;
    }

    return inter;
}