#include <iostream>

#include <thread>

#include <geometry/plane.hpp>
#include <geometry/sphere.hpp>
#include <geometry/triangle.hpp>
#include <scene/scene.hpp>

#include <core/core.hpp>

#include <image/tonemapping.hpp>

using namespace std;

int main() {

    SceneProps props {
        .viewportWidth = 512,
        .viewportHeight = 512,
        .antialiasingFactor = 8,
        .threads = std::thread::hardware_concurrency()
    };

    Camera cam(
        Vector3(-1,0,0), Vector3(0, 1, 0), Vector3(0, 0, 3), Vector3(0, 0, -3.5), 
        props.viewportWidth, props.viewportHeight
    );

    Scene sc(props, cam);

    auto pL = make_shared<Plane> (1, Vector3(1, 0, 0), RGB(1, 0, 0));
    auto pR = make_shared<Plane> (1, Vector3(-1, 0, 0), RGB(0, 1, 0));
    auto pF = make_shared<Plane> (1, Vector3(0, 1, 0), RGB(0.5, 0.5, 0.5));
    auto pC = make_shared<Plane> (1, Vector3(0, -1, 0), RGB(0.5, 0.5, 0.5));
    auto pB = make_shared<Plane> (1, Vector3(0, 0, -1), RGB(0.5, 0.5, 0.5));

    auto sL = make_shared<Sphere>(Vector3(-0.5,-0.7,0.5), 0.2, RGB(1, 1, 0));
    auto sR = make_shared<Sphere>(Vector3(0.5, -0.7, 0.25), 0.4, RGB(0, 1, 1));

    auto light = make_shared<Light>(Vector3(0,0.5,0), RGB(1,1,1));

    // auto sTri = make_shared<Triangle>(Vector3(0, 0, 0.5), Vector3(0, 1, 0.5), Vector3(1, 0, 0.5), RGB(0.72, 0.57, 0.62));

    sc.addPrimitive(pL);
    sc.addPrimitive(pR);
    sc.addPrimitive(pF);
    sc.addPrimitive(pC);
    sc.addPrimitive(pB);
    sc.addPrimitive(sL);
    sc.addPrimitive(sR);

    sc.addLight(light);
    //sc.addPrimitive(sTri);

    Image img(props.viewportWidth, props.viewportHeight);
    
    cout << "Rendering... " << flush;
    auto ms = measureTime<std::chrono::milliseconds>( [&](){ img = sc.drawScene(); } );

    cout << "done (" << ms << " ms)." << endl;

    cout << "Tonemapping image..." << flush;

    Image tmImg = tonemapping::gamma(img, 1 / 2.2);

    cout << "Writing image... " << flush;

    auto msimg = measureTime<std::chrono::milliseconds>( [&](){ tmImg.writeToBMP("test.bmp"); } );

    cout << "done (" << msimg << " ms)." << endl;
}