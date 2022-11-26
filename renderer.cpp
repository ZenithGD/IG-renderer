#include <iostream>

#include <thread>


#include <geometry/CSG.hpp>
#include <geometry/plane.hpp>
#include <geometry/sphere.hpp>
#include <geometry/triangle.hpp>
#include <scene/scene.hpp>
#include <pathtracer/pathtracing.hpp>
#include <core/core.hpp>

#include <image/tonemapping.hpp>

using namespace std;

int main() {

    SceneProps props {
        .viewportWidth = 300,
        .viewportHeight = 300,
        .antialiasingFactor = 64,
        .threads = std::thread::hardware_concurrency(),
        .bounces = 5
    };

    Camera cam(
        Vector3(-1,0,0), Vector3(0, 1, 0), Vector3(0, 0, 3.5), Vector3(0, 0, -3.5), 
        props.viewportWidth, props.viewportHeight
    );

    Scene sc(props, cam);

    BSDF bsdfPL(RGB(1, 0, 0));
    BSDF bsdfPR(RGB(0, 1, 0));
    BSDF bsdfL(RGB(0.5, 0.65, 0.8), RGB(0.2, 0.2, 0.2));
    BSDF bsdfR(RGB(), RGB(0.1, 0.1, 0.1), RGB(0.9, 0.9, 0.9), 1.5);

    auto pL = make_shared<Plane> (1, Vector3(1, 0, 0), bsdfPL);
    auto pR = make_shared<Plane> (1, Vector3(-1, 0, 0), bsdfPR);
    auto pF = make_shared<Plane> (1, Vector3(0, 1, 0), RGB(0.5, 0.5, 0.5));
    auto pC = make_shared<Plane> (1, Vector3(0, -1, 0), RGB(1, 1, 1));
    auto pB = make_shared<Plane> (Vector3(0,0,-5), Vector3(0,1,-5), Vector3(1,0,-5), RGB(1, 1, 1));

    auto sL = make_shared<Sphere>(Vector3(-0.5, -0.7, 0.25), 0.3, bsdfL);
    auto sR = make_shared<Sphere>(Vector3( 0.5, -0.7, -0.25), 0.3, bsdfR);
    auto sR2 = make_shared<Sphere>(Vector3( 0.5, -0.7, 3), 0.3, bsdfR);
    auto sR3 = make_shared<Sphere>(Vector3( 0.5, 0.4, 1), 0.3, bsdfR);
    auto sR4 = make_shared<Sphere>(Vector3( 0.3, 0, -0.85), 0.3, bsdfR);
    
    auto sCSG1 = make_shared<CSG>(sR, sR2, CSGOperation::Intersection, bsdfR);

    /*
    auto s1 = make_shared<Sphere>(Vector3(0.0, 0.3, 0.5), 0.4, RGB(1, 0, 0.5));
    auto s2 = make_shared<Sphere>(Vector3(0.0, 0.3, 0.5), 0.35, RGB(1, 0, 0.5));
    auto s3 = make_shared<Sphere>(Vector3(0.2, 0.3, 0.4), 0.3, RGB(1, 0, 0.5));

    auto sCSG1 = make_shared<CSG>(s1, s2, CSGOperation::Difference, RGB(1, 0, 0.5));
    auto sCSG2 = make_shared<CSG>(sCSG1, s3, CSGOperation::Difference, RGB(1, 0, 0.5));
    */
    
    auto light  = make_shared<PointLight>(Vector3(0.0, 0.5, 0), RGB(1,1,1));
    auto light2 = make_shared<PointLight>(Vector3(0.0, 0.5, -4), RGB(1,1,1));
    // auto sTri = make_shared<Triangle>(Vector3(0, 0, 0.5), Vector3(0, 1, 0.5), Vector3(1, 0, 0.5), RGB(0.72, 0.57, 0.62));

    sc.addPrimitive(pL);
    sc.addPrimitive(pR);
    sc.addPrimitive(pF);
    sc.addPrimitive(pC);
    //sc.addPrimitive(pB);
    sc.addPrimitive(sL);
    sc.addPrimitive(sR);
    sc.addPrimitive(sR2);
    sc.addPrimitive(sR3);
    sc.addPrimitive(sR4);
    //sc.addPrimitive(sR2);

    //sc.addPrimitive(s1);
    //sc.addPrimitive(sCSG1);

    sc.addLight(light);
    //sc.addLight(light2);
    //sc.addPrimitive(sTri);

    Image img(props.viewportWidth, props.viewportHeight);
    
    cout << "Rendering... " << flush;
    auto ms = measureTime<std::chrono::milliseconds>( 
        [&](){ img = sc.drawScene(
            [&](const Scene& sc) -> Image { 
                return pathTracing(sc); 
            }
        );
    });

    cout << "done (" << ms << " ms)." << endl;

    cout << "Tonemapping image..." << flush;

    Image tmImg = tonemapping::gamma(img, 2.2);

    cout << "Writing image... " << flush;

    auto msimg = measureTime<std::chrono::milliseconds>( [&](){ tmImg.writeToBMP("test.bmp"); } );

    cout << "done (" << msimg << " ms)." << endl;
}