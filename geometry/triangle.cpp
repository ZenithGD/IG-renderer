#include <geometry/triangle.hpp>

#include <core/core.hpp>

bool Triangle::insideOutsideTest(Vector3 point) const {

    Vector3 edge0 = pointB - pointA; 
    Vector3 edge1 = pointC - pointB; 
    Vector3 edge2 = pointA - pointC; 
    Vector3 C0 = point - pointA; 
    Vector3 C1 = point - pointB; 
    Vector3 C2 = point - pointC; 

    //cout << normal << ", " << c << endl;
/*
    cout << "point : " << point << endl;
    if ( dot(normal, cross(edge0, C0)) > 0.0 )
        cout << "C0: " << dot(normal, cross(edge0, C0)) << endl;

    if ( dot(normal, cross(edge1, C1)) > 0.0 )
        cout << "C1: " << dot(normal, cross(edge1, C1)) << endl;

    if ( dot(normal, cross(edge2, C2)) > 0.0 )
        cout << "C2: " << dot(normal, cross(edge2, C2)) << endl;
        */

    bool a = 
        dot(normal, cross(edge0, C0)) > 0.0 && 
        dot(normal, cross(edge1, C1)) > 0.0 && 
        dot(normal, cross(edge2, C2)) > 0.0;  //P is inside the triangle 
    return a;
}

Intersection Triangle::intersection(const Ray& r, double minT, double maxT) {
    Intersection inter;
    //cout << "c: " << c << endl;
    //Solve for t
    double t = -((c + dot(r.origin, normal)) / dot(r.direction, normal));

    // Verify if the plane not intersect behind of the r.origin
    if (t < minT + INTERSECTION_TOLERANCE || t > maxT) {
        inter.intersects = false;
    }
    else {
        inter.intersects = insideOutsideTest(r(t));
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

tuple<double, double> Triangle::getUVCoords(const Vector3& point) const {
    // UV coordinates on a plane are equal to the length of the projection
    // to the U and V parametric axes of the plane.
    return make_tuple<double, double>( dot(uAxis, point), dot(vAxis, point) );
}