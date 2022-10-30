#include <iostream>

#include <geometry/plane.hpp>
#include <geometry/sphere.hpp>
#include <geometry/triangle.hpp>
#include <scene/scene.hpp>

using namespace std;

int main() {

/*  
    auto s = make_shared<Sphere>(Vector3(-7.04,5.16,2), 1.5, RGB(1, 0, 1));

    Ray r(Vector3(0,0,0), Vector3(-3.97,3.12648,1.48));
    
    auto si = s->intersection(r);
    cout << ", Point : " << si.point << ", Normal : " << si.normal << endl;

    auto p = make_shared<Plane> (-5, Vector3(-5, 2.5, 5), RGB(1, 1, 0));

    Ray r2(Vector3(1,1,1), Vector3(0,2,1));

    auto planeInt = p->intersection(r2);

    cout << "Intersection: "  << planeInt.point << ", Normal : " << planeInt.normal << endl;
*/  

    Camera cam(Vector3(-1,0,0), Vector3(0, 1, 0), Vector3(0, 0, 3), Vector3(0, 0, -3.5), 256, 256);
    Scene sc(cam);

    auto pL = make_shared<Plane> (1, Vector3(1, 0, 0), RGB(1, 0, 0));
    auto pR = make_shared<Plane> (1, Vector3(-1, 0, 0), RGB(0, 1, 0));
    auto pF = make_shared<Plane> (1, Vector3(0, 1, 0), RGB(0.5, 0.5, 0.5));
    auto pC = make_shared<Plane> (1, Vector3(0, -1, 0), RGB(0.5, 0.5, 0.5));
    auto pB = make_shared<Plane> (1, Vector3(0, 0, -1), RGB(0.5, 0.5, 0.5));

    auto sL = make_shared<Sphere>(Vector3(-0.5,-0.7,0.25), 0.3, RGB(0.5, 0, 0));
    auto sR = make_shared<Sphere>(Vector3(0.5, -0.7, 0.25), 0.3, RGB(0, 0, 0.5));

    auto sTri = make_shared<Triangle>(Vector3(0, 0, 0.5), Vector3(0, 1, 0.5), Vector3(1, 0, 0.5), RGB(0.72, 0.57, 0.62));

    sc.addPrimitive(sTri);

    Image img = sc.drawScene();

    img.writeToBMP("test.bmp");
}