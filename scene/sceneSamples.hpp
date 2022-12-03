#include <geometry/CSG.hpp>
#include <geometry/plane.hpp>
#include <geometry/sphere.hpp>
#include <geometry/triangle.hpp>
#include <scene/scene.hpp>
#include <pathtracer/pathtracing.hpp>
#include <core/core.hpp>
#include <material/SimpleBRDF.hpp>
#include <material/PhongBRDF.hpp>
#include <material/FresnelBRDF.hpp>

#include <image/tonemapping.hpp>

Scene cornellDiffuse(const SceneProps& props) {

    Camera cam(
        Vector3(-1,0,0), Vector3(0, 1, 0), Vector3(0, 0, 3), Vector3(0, 0, -3.5), 
        props.viewportWidth, props.viewportHeight
    );

    Scene sc(props, cam);

    auto BRDFPL = make_shared<SimpleBRDF>(RGB(1, 0, 0));
    auto BRDFPR = make_shared<SimpleBRDF>(RGB(0, 1, 0));
    auto BRDFL = make_shared<SimpleBRDF>(RGB(0.6, 0.75, 0.8));
    auto BRDFR = make_shared<SimpleBRDF>(RGB(0.6, 0.7, 0.2));
    auto fullReflect = make_shared<SimpleBRDF>(RGB(0.1, 0.1, 0.1), RGB(0.9, 0.9, 0.9));
    auto gray = make_shared<SimpleBRDF>(RGB(0.7, 0.7, 0.7));

    auto pL = make_shared<Plane> (1, Vector3(1, 0, 0), BRDFPL);
    auto pR = make_shared<Plane> (1, Vector3(-1, 0, 0), BRDFPR);
    auto pF = make_shared<Plane> (1, Vector3(0, 1, 0), gray);
    auto pC = make_shared<Plane> (1, Vector3(0, -1, 0), gray);
    auto pB = make_shared<Plane> (1, Vector3(0, 0, -1), gray);
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

    return sc;
}

Scene cornellMatVariations(const SceneProps& props) {

    Camera cam(
        Vector3(-1,0,0), Vector3(0, 1, 0), Vector3(0, 0, 3), Vector3(0, 0, -3.5), 
        props.viewportWidth, props.viewportHeight
    );

    Scene sc(props, cam);

    auto BRDFPL = make_shared<SimpleBRDF>(RGB(1, 0, 0));
    auto BRDFPR = make_shared<SimpleBRDF>(RGB(0, 1, 0));
    auto BRDFL = make_shared<FresnelBRDF>(RGB(1, 1, 1), RGB(1, 1, 1), 1.5);
    auto BRDFR = make_shared<FresnelBRDF>(RGB(1, 1, 1), RGB(1, 1, 1), 1.5);
    auto fullReflect = make_shared<SimpleBRDF>(RGB(0.1, 0.1, 0.1), RGB(0.9, 0.9, 0.9));
    auto gray = make_shared<SimpleBRDF>(RGB(0.7, 0.7, 0.7));

    auto pL = make_shared<Plane> (1, Vector3(1, 0, 0), BRDFPL);
    auto pR = make_shared<Plane> (1, Vector3(-1, 0, 0), BRDFPR);
    auto pF = make_shared<Plane> (1, Vector3(0, 1, 0), gray);
    auto pC = make_shared<Plane> (1, Vector3(0, -1, 0), gray);
    auto pB = make_shared<Plane> (1, Vector3(0, 0, -1), gray);
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

    return sc;
}