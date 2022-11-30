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
#include <material/SimpleBRDF.hpp>

using namespace std;

int main() {

    SceneProps props {
        .viewportWidth = 300,
        .viewportHeight = 300,
        .antialiasingFactor = 32,
        .threads = std::thread::hardware_concurrency(),
        .bounces = 5
    };

    Camera cam(
        Vector3(-1,0,0), Vector3(0, 1, 0), Vector3(0, 0, 3), Vector3(0, 0, -3.5), 
        props.viewportWidth, props.viewportHeight
    );

    Scene sc(props, cam);

    auto BRDFPL = make_shared<SimpleBRDF>(RGB(1, 0, 0));
    auto BRDFPR = make_shared<SimpleBRDF>(RGB(0, 1, 0));
    auto BRDFL = make_shared<SimpleBRDF>(RGB(0.5, 0.65, 0.8), RGB(0.2, 0.2, 0.2));
    auto BRDFR = make_shared<SimpleBRDF>(RGB(0.0, 0.0, 0.1), RGB(0.1, 0.1, 0.1), RGB(0.8, 0.8, 0.8), 1.5);
    auto fullReflect = make_shared<SimpleBRDF>(RGB(0.1, 0.1, 0.1), RGB(0.9, 0.9, 0.9));
    auto gray = make_shared<SimpleBRDF>(RGB(0.7, 0.7, 0.7));

    auto pL = make_shared<Plane> (1, Vector3(1, 0, 0), BRDFPL);
    auto pR = make_shared<Plane> (1, Vector3(-1, 0, 0), BRDFPR);
    auto pF = make_shared<Plane> (1, Vector3(0, 1, 0), gray);
    auto pC = make_shared<Plane> (1, Vector3(0, -1, 0), gray);
    auto pB = make_shared<Plane> (1, Vector3(0, 0, -1), fullReflect);
    auto pBack = make_shared<Plane> (-4, Vector3(0, 0, 1), gray);

    auto sL = make_shared<Sphere>(Vector3(-0.5, -0.7, 0.25), 0.3, BRDFL);
    auto sR = make_shared<Sphere>(Vector3( 0.5, -0.7, -0.25), 0.3, BRDFR);
    auto sR4 = make_shared<Sphere>(Vector3( 0.3, 0, 0.6), 0.3, BRDFR);

    /*
    auto s1 = make_shared<Sphere>(Vector3(0.0, 0.3, 0.5), 0.4, RGB(1, 0, 0.5));
    auto s2 = make_shared<Sphere>(Vector3(0.0, 0.3, 0.5), 0.35, RGB(1, 0, 0.5));
    auto s3 = make_shared<Sphere>(Vector3(0.2, 0.3, 0.4), 0.3, RGB(1, 0, 0.5));

    auto sCSG1 = make_shared<CSG>(s1, s2, CSGOperation::Difference, RGB(1, 0, 0.5));
    auto sCSG2 = make_shared<CSG>(sCSG1, s3, CSGOperation::Difference, RGB(1, 0, 0.5));
    */
    
    auto light  = make_shared<PointLight>(Vector3(0.0, 0.5, 0), RGB(1,1,1));
    //auto sTri = make_shared<Triangle>(Vector3(-1, 0, 0.45), Vector3(1, 0, 0.45), Vector3(0, 0.25, 0.75), RGB(0.72, 0.57, 0.62));

    sc.addPrimitive(pL);
    sc.addPrimitive(pR);
    sc.addPrimitive(pF);
    sc.addPrimitive(pC);
    sc.addPrimitive(pB);
    //sc.addPrimitive(pBack);
    sc.addPrimitive(sL);
    sc.addPrimitive(sR);
    //sc.addPrimitive(sR4);
    //sc.addPrimitive(sR2);

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