#include <iostream>

#include <geometry/ray.hpp>
#include <geometry/plane.hpp>

using namespace std;

int main() {

    Sphere s(Vector3(-7.04,5.16,2), 1.5);

    Ray r(Vector3(0,0,0), Vector3(-3.97,3.12648,1.48));
    
    auto si = r.intersection(s);
    cout << "Intersections : " << si.intersections << ", Point : " << si.point << ", Normal : " << si.normalVector << endl;

    Plane p(-5, Vector3(-5, 2.5, 5));

    Ray r2(Vector3(1,1,1), Vector3(0,2,1));

    auto planeInt = r2.intersection(p);

    cout << "Intersection: "  << planeInt.point << ", Normal : " << planeInt.normal << endl;

}