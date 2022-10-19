#include <iostream>

#include <geometry/ray.hpp>
#include <geometry/plane.hpp>

using namespace std;

int main() {

    Sphere s(Vector3(1,1,1), 1);

    Ray r(Vector3(), Vector3(2, 3.5, 1));
    
    auto si = r.intersection(s);
    cout << "Intersections : " << si.intersections << ", Point : " << r(si.intersectionPoint) << ", Normal : " << si.normalVector << endl;


    Plane p(-5, Vector3(1, 1, 1));

    Ray r2(Vector3(), Vector3(1,1,1));

    auto planeInt = r2.intersection(p);

    cout << "Intersection: " << r(planeInt.t) << ", Normal : " << planeInt.normal << endl;

}