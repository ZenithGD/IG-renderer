#include <geometry/triangle.hpp>
#include <math/interpolation.hpp>

#include <core/core.hpp>

bool Triangle::insideOutsideTest(Vector3 point, double& u, double& v) const {

    Vector3 edge0 = pointB.pos - pointA.pos; 
    Vector3 edge1 = pointC.pos - pointB.pos; 
    Vector3 edge2 = pointA.pos - pointC.pos; 
    Vector3 C0 = point - pointA.pos; 
    Vector3 C1 = point - pointB.pos; 
    Vector3 C2 = point - pointC.pos; 

    double a0 = dot(normal, cross(edge0, C0));
    double a1 = dot(normal, cross(edge1, C1));
    double a2 = dot(normal, cross(edge2, C2));

    u = a1 / n2;
    v = a2 / n2;

    //P is inside the triangle 
    return a0 > 0.0 && a1 > 0.0 && a2 > 0.0;
}

Intersection Triangle::intersection(const Ray& r, double minT, double maxT) {
    Intersection inter;
    //cout << "c: " << c << endl;
    //Solve for t
    double t = -((c + dot(r.origin, normal)) / dot(r.direction, normal));

    double u, v;

    // Verify if the plane not intersect behind of the r.origin
    if (t < minT + INTERSECTION_TOLERANCE || t > maxT) {
        inter.intersects = false;
    }
    else {
        inter.intersects = insideOutsideTest(r(t), u, v);
    }

    if ( inter.intersects ) {
        inter.brdf = brdf;

        Vector2 uvPoint = barInterpolation<Vector2>(pointA.uv, pointB.uv, pointC.uv, u, v);

        //cout << uAxis << ", " << vAxis << endl;

        inter.u = uvPoint.x;
        inter.v = uvPoint.y;

        inter.intersections.emplace(
            t, 
            normalize(barInterpolation<Vector3>(pointA.normal, pointB.normal, pointC.normal, inter.u, inter.v))
        );
    }
    return inter;
}
