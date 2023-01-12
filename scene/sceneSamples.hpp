#include <geometry/CSG.hpp>
#include <geometry/plane.hpp>
#include <geometry/sphere.hpp>
#include <geometry/triangle.hpp>
#include <geometry/triangleMesh.hpp>
#include <geometry/rectangle.hpp>
#include <geometry/box.hpp>
#include <scene/scene.hpp>
#include <pathtracer/pathtracing.hpp>
#include <core/core.hpp>

#include <material/SimpleBRDF.hpp>
#include <material/PhongBRDF.hpp>
#include <material/FresnelBRDF.hpp>
#include <material/Emitter.hpp>
#include <material/TextureBRDF.hpp>

#include <texture/checkerBoard.hpp>
#include <texture/noiseTexture.hpp>
#include <texture/imageTexture.hpp>

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
    auto gray = make_shared<SimpleBRDF>(RGB(0.7, 0.7, 0.7));
    auto pL = make_shared<Plane> (1, Vector3(1, 0, 0), BRDFPL);
    auto pR = make_shared<Plane> (1, Vector3(-1, 0, 0), BRDFPR);
    auto pF = make_shared<Plane> (1, Vector3(0, 1, 0), gray);
    auto pC = make_shared<Plane> (1, Vector3(0, -1, 0), gray);
    auto pB = make_shared<Plane> (1, Vector3(0, 0, -1), gray);
    auto pBack = make_shared<Plane> (-4, Vector3(0, 0, 1), gray);


    auto sL = make_shared<Sphere>(Vector3(-0.5, -0.7, 0.25), 0.3, BRDFL);
    auto sR = make_shared<Sphere>(Vector3( 0.5, -0.7, -0.25), 0.3, BRDFR);
    auto light  = make_shared<PointLight>(Vector3(0.0, 0.5, 0), RGB(1,1,1));

    sc.addPrimitive(pL);
    sc.addPrimitive(pR);
    sc.addPrimitive(pF);
    sc.addPrimitive(pC);
    sc.addPrimitive(pB);

    sc.addPrimitive(sL);
    sc.addPrimitive(sR);

    sc.addLight(light);

    return sc;
}

Scene cornellKittenTexture(const SceneProps& props) {

    Camera cam(
        Vector3(-1,0,0), Vector3(0, 1, 0), Vector3(0, 0, 3), Vector3(0, 0, -3.5), 
        props.viewportWidth, props.viewportHeight
    );

    Scene sc(props, cam);

    auto BRDFPL = make_shared<SimpleBRDF>(RGB(1, 0, 0));
    auto BRDFPR = make_shared<SimpleBRDF>(RGB(0, 1, 0));
    auto BRDFL = make_shared<SimpleBRDF>(RGB(0.6, 0.75, 0.8));
    auto BRDFR = make_shared<SimpleBRDF>(RGB(0.6, 0.7, 0.2));
    auto gray = make_shared<SimpleBRDF>(RGB(0.7, 0.7, 0.7));
    auto pL = make_shared<Plane> (1, Vector3(1, 0, 0), BRDFPL);
    auto pR = make_shared<Plane> (1, Vector3(-1, 0, 0), BRDFPR);
    auto pF = make_shared<Plane> (1, Vector3(0, 1, 0), gray);
    auto pC = make_shared<Plane> (1, Vector3(0, -1, 0), gray);
    auto pB = make_shared<Plane> (1, Vector3(0, 0, -1), gray);
    auto pBack = make_shared<Plane> (-4, Vector3(0, 0, 1), gray);

    Vector3 v0(0, 0.2, 0.5), v1(-0.5, -0.7, 0.25), v2(0.5, -0.7, -0.25);

    auto normal = cross(v1 - v0, v2 - v0).normalized();

    VertexInfo vi0{
        .pos = v0,
        .normal = normal,
        .uv = Vector2(0.5, 0)
    };

    VertexInfo vi1{
        .pos = v1,
        .normal = normal,
        .uv = Vector2(0, 1)
    };

    VertexInfo vi2{
        .pos = v2,
        .normal = normal,
        .uv = Vector2(1, 1)
    };

    auto img = Image::readPPM("assets/kitten.ppm");
    auto tex1 = make_shared<ImageTexture>(img);
    auto brdfKitten = make_shared<TextureBRDF>(tex1, 1);

    auto tri = make_shared<Triangle>(vi0, vi1, vi2, brdfKitten);

    auto sL = make_shared<Sphere>(Vector3(-0.5, -0.7, 0.25), 0.3, BRDFL);
    auto sR = make_shared<Sphere>(Vector3( 0.5, -0.7, -0.25), 0.3, BRDFR);
    auto sR4 = make_shared<Sphere>(Vector3(0, -0.2, 0.5), 0.1, BRDFR);

    auto light  = make_shared<PointLight>(Vector3(0.0, 0.5, 0), RGB(1,1,1));

    sc.addPrimitive(pL);
    sc.addPrimitive(pR);
    sc.addPrimitive(pF);
    sc.addPrimitive(pC);
    sc.addPrimitive(pB);
    sc.addLight(light);
    sc.addPrimitive(tri);

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
    auto BRDFL = make_shared<SimpleBRDF>(RGB(0.1, 0.4, 0.5), RGB(0.5,0.5,0.5), RGB());
    auto BRDFR = make_shared<FresnelBRDF>(RGB(1,1,1), RGB(1, 1, 1), 1.5);
    auto gray = make_shared<SimpleBRDF>(RGB(0.5, 0.5, 0.5));

    auto pL = make_shared<Plane> (1, Vector3(1, 0, 0), BRDFPL);
    auto pR = make_shared<Plane> (1, Vector3(-1, 0, 0), BRDFPR);
    auto pF = make_shared<Plane> (1, Vector3(0, 1, 0), gray);
    auto pC = make_shared<Plane> (1, Vector3(0, -1, 0), gray);
    auto pB = make_shared<Plane> (1, Vector3(0, 0, -1), gray);
    auto pBack = make_shared<Plane> (-4, Vector3(0, 0, 1), gray);

    auto sL = make_shared<Sphere>(Vector3(-0.5, -0.7, 0.25), 0.3, BRDFL);
    auto sR = make_shared<Sphere>(Vector3( 0.5, -0.7, -0.25), 0.3, BRDFR);
    auto light  = make_shared<PointLight>(Vector3(0.0, 0.5, 0), RGB(1,1,1));

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

Scene cornellEmission(const SceneProps& props) {

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
    auto emission = make_shared<Emitter>(RGB(1, 1, 1));

    auto pL = make_shared<Plane> (1, Vector3(1, 0, 0), BRDFPL);
    auto pR = make_shared<Plane> (1, Vector3(-1, 0, 0), BRDFPR);
    auto pF = make_shared<Plane> (1, Vector3(0, 1, 0), gray);
    auto pC = make_shared<Plane> (1, Vector3(0, -1, 0), gray);
    auto pB = make_shared<Plane> (1, Vector3(0, 0, -1), gray);
    auto pLight = make_shared<RectangleXZ>(1.5, 1.5, Vector3(0, 0.999, 0), emission);

    auto sL = make_shared<Sphere>(Vector3(-0.5, -0.7, 0.25), 0.3, BRDFL);
    auto sR = make_shared<Sphere>(Vector3( 0.5, -0.7, -0.25), 0.3, BRDFR);
    auto sR4 = make_shared<Sphere>(Vector3( 0.3, 0, 0.6), 0.3, BRDFR);
    
    auto light  = make_shared<PointLight>(Vector3(0.0, 0.5, 0), RGB(1,1,1));
    //auto sTri = make_shared<Triangle>(Vector3(-1, 0, 0.45), Vector3(1, 0, 0.45), Vector3(0, 0.25, 0.75), RGB(0.72, 0.57, 0.62));

    sc.addPrimitive(pL);
    sc.addPrimitive(pR);
    sc.addPrimitive(pF);
    sc.addPrimitive(pC);
    sc.addPrimitive(pB);

    sc.addPrimitive(sL);
    sc.addPrimitive(sR);
    sc.addPrimitive(pLight);
    //sc.addLight(light);

    return sc;
}

Scene cornellTextured(const SceneProps& props) {

    Camera cam(
        Vector3(-1,0,0), Vector3(0, 1, 0), Vector3(0, 0, 3), Vector3(0, 0, -3.5), 
        props.viewportWidth, props.viewportHeight
    );

    Scene sc(props, cam);

    auto BRDFPL = make_shared<SimpleBRDF>(RGB(1, 0, 0));
    auto BRDFPR = make_shared<SimpleBRDF>(RGB(0, 1, 0));
    auto BRDFL = make_shared<FresnelBRDF>(RGB(0, 1, 1), RGB(0.6, 1, 1), 1.5);
    auto BRDFR = make_shared<FresnelBRDF>(RGB(1, 0, 0.8), RGB(1, 0.8, 0.9), 1.5);
    auto fullReflect = make_shared<SimpleBRDF>(RGB(0.1, 0.1, 0.1), RGB(0.9, 0.9, 0.9));
    
    auto tex = make_shared<Checkerboard>(RGB(1,1,1), RGB(0.7, 0.7, 0.7), 0.2, 0.2);
    auto texture = make_shared<TextureBRDF>(tex, 1);
    
    auto pL = make_shared<Plane> (1, Vector3(1, 0, 0), BRDFPL);
    auto pR = make_shared<Plane> (1, Vector3(-1, 0, 0), BRDFPR);
    auto pF = make_shared<Plane> (1, Vector3(0, 1, 0), texture);
    auto pC = make_shared<Plane> (1, Vector3(0, -1, 0), texture);
    auto pB = make_shared<Plane> (1, Vector3(0, 0, -1), texture);
    auto pBack = make_shared<Plane> (4, Vector3(0, 0, 1), texture);

    auto sL = make_shared<Sphere>(Vector3(-0.5, -0.7, 0.25), 0.3, BRDFL);
    auto sR = make_shared<Sphere>(Vector3( 0.5, -0.7, -0.25), 0.3, BRDFR);
    auto sR4 = make_shared<Sphere>(Vector3( 0.3, 0, 0.6), 0.3, BRDFR);
    
    auto light  = make_shared<PointLight>(Vector3(0.0, 0.5, 0), RGB(1,1,1));
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


Scene cornellEnvMap(const SceneProps& props) {

    Camera cam(
        Vector3(-2,0,0), Vector3(0, 1, 0), Vector3(0, 0, 3), Vector3(0, 0, -3.5), 
        props.viewportWidth, props.viewportHeight
    );

    auto img = Image::readPPM("assets/envmaps/brown_photostudio.ppm");
    auto tex1 = make_shared<ImageTexture>(img);
    
    auto tex2 = make_shared<Checkerboard>(RGB(1, 0, 1) , RGB(1, 1, 1), 0.2, 0.2);
    auto texture2 = make_shared<TextureBRDF>(tex2, 1);

    auto sphereBRDF = make_shared<SimpleBRDF>(RGB(0, 0, 0), RGB(0.9, 0.9, 0.9), RGB(0.1, 0.1, 0.1), 1.5);
 
    auto pB = make_shared<Plane> (1, Vector3(0, 0, -1), texture2);
    auto pL = make_shared<Plane> (1, Vector3(1, 0, 0), texture2);
    auto pF = make_shared<Plane> (1, Vector3(0, 1, 0), texture2);
    auto sL = make_shared<Sphere>(Vector3(0, -0.5, 0.25), 0.5, sphereBRDF);
    //auto mirror = make_shared<Triangle>(Vector3());
    Scene sc(props, cam, tex1);

    //sc.addPrimitive(pL);
    sc.addPrimitive(pF);
    //sc.addPrimitive(pB);
    sc.addPrimitive(sL);

    return sc;
}

Scene pyramidScene(const SceneProps& props) {

    Camera cam(
        Vector3(-1,0,0), Vector3(0, 1, 0), Vector3(0, 0, 3), Vector3(0, 0, -3.5), 
        props.viewportWidth, props.viewportHeight
    );

     Scene sc(props, cam);

    auto BRDFPL = make_shared<SimpleBRDF>(RGB(1, 0, 0));
    auto BRDFPR = make_shared<SimpleBRDF>(RGB(0, 1, 0));
    auto BRDFP = make_shared<SimpleBRDF>(RGB(0, 0, 1));
    auto BRDFL = make_shared<SimpleBRDF>(RGB(0.6, 0.75, 0.8));
    auto BRDFR = make_shared<SimpleBRDF>(RGB(0.6, 0.7, 0.2));
    auto gray = make_shared<SimpleBRDF>(RGB(0.7, 0.7, 0.7));
    auto pL = make_shared<Plane> (1, Vector3(1, 0, 0), BRDFPL);
    auto pR = make_shared<Plane> (1, Vector3(-1, 0, 0), BRDFPR);
    auto pF = make_shared<Plane> (1, Vector3(0, 1, 0), gray);
    auto pC = make_shared<Plane> (1, Vector3(0, -1, 0), gray);
    auto pB = make_shared<Plane> (1, Vector3(0, 0, -1), gray);
    auto pBack = make_shared<Plane> (-4, Vector3(0, 0, 1), gray);

    vector<Vector3> normals, vertices;
    vector<Vector2> uvCoords;
    vector<unsigned int> index;

    Vector3 v0(0, -0.2, 0), v1(-0.4, 0.2, 1), v2(0, -0.7, 1), v3(0.4, 0.2, 1);

    vertices.push_back(v0);
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);

    // face 0, left
    Vector3 face0 = cross(v1 - v0, v2 - v0).normalized();
    index.push_back(0);
    normals.push_back(face0);
    index.push_back(1);
    normals.push_back(face0);
    index.push_back(2);
    normals.push_back(face0);

    // face 1, right
    Vector3 face1 = cross(v2 - v0, v3 - v0).normalized();
    index.push_back(0);
    normals.push_back(face1);
    index.push_back(2);
    normals.push_back(face1);
    index.push_back(3);
    normals.push_back(face1);

    // face 1, up
    Vector3 face2 = cross(v3 - v0, v1 - v0).normalized();
    index.push_back(0);
    normals.push_back(face2);
    index.push_back(3);
    normals.push_back(face2);
    index.push_back(1);
    normals.push_back(face2);


    uvCoords.push_back(Vector2(0, 0));
    uvCoords.push_back(Vector2(0, 1));
    uvCoords.push_back(Vector2(1, 1));
    uvCoords.push_back(Vector2(1, 0));

    //auto img = Image::readPPM("assets/wmt.ppm");
    //auto tex1 = make_shared<ImageTexture>(img);
    //auto brdfImg = make_shared<TextureBRDF>(tex1, 1);

    auto tri = make_shared<TriangleMesh>(vertices, index, normals, uvCoords, BRDFP);

    auto sL = make_shared<Sphere>(Vector3(-0.5, -0.7, 0.25), 0.3, BRDFL);
    auto sR = make_shared<Sphere>(Vector3( 0.5, -0.7, -0.25), 0.3, BRDFR);
    auto sR4 = make_shared<Sphere>(Vector3(0, -0.2, 0.5), 0.1, BRDFR);

    auto light  = make_shared<PointLight>(Vector3(0.0, 0.5, 0), RGB(1,1,1));
    
    sc.addPrimitive(pL);
    sc.addPrimitive(pR);
    sc.addPrimitive(pF);
    sc.addPrimitive(pC);
    sc.addPrimitive(pB);

    sc.addLight(light);
    //sc.addLight(light2);
    sc.addPrimitive(tri);

    return sc;
}


Scene cornellNoise(const SceneProps& props) {

    Camera cam(
        Vector3(-1,0,0), Vector3(0, 1, 0), Vector3(0, 0, 3), Vector3(0, 0, -3.5), 
        props.viewportWidth, props.viewportHeight
    );

    Scene sc(props, cam);

    auto BRDFPL = make_shared<SimpleBRDF>(RGB(1, 0, 0));
    auto BRDFPR = make_shared<SimpleBRDF>(RGB(0, 1, 0));
    auto BRDFL = make_shared<SimpleBRDF>(RGB(0.6, 0.75, 0.8));
    auto BRDFR = make_shared<SimpleBRDF>(RGB(0.6, 0.7, 0.2));
    auto gray = make_shared<SimpleBRDF>(RGB(0.7, 0.7, 0.7));
    
    auto noise = make_shared<FractalNoise>(16, 5, 1, 1);
    auto tNoise = make_shared<TurbulentNoise>(noise);
    auto tex = make_shared<NoiseTexture>(tNoise, RGB(0,0,0), RGB(1,1,1));
    auto texture = make_shared<TextureBRDF>(tex, 1);

    auto wNoise = make_shared<WoodyNoise>(noise, 5);
    auto tex2 = make_shared<NoiseTexture>(wNoise, RGB(0,0,0), RGB(1,1,1));
    auto texture2 = make_shared<TextureBRDF>(tex2, 1);

    auto pL = make_shared<Plane> (1, Vector3(1, 0, 0), BRDFPL);
    auto pR = make_shared<Plane> (1, Vector3(-1, 0, 0), BRDFPR);
    auto pF = make_shared<Plane> (1, Vector3(0, 1, 0), gray);
    auto pC = make_shared<Plane> (1, Vector3(0, -1, 0), gray);
    auto pB = make_shared<Plane> (1, Vector3(0, 0, -1), texture);

    auto sL = make_shared<Sphere>(Vector3(-0.5, -0.7, 0.25), 0.3, texture2);
    auto sR = make_shared<Sphere>(Vector3( 0.5, -0.7, -0.25), 0.3, texture2);
    auto light  = make_shared<PointLight>(Vector3(0.0, 0.5, 0), RGB(1,1,1));

    sc.addPrimitive(pL);
    sc.addPrimitive(pR);
    sc.addPrimitive(pF);
    sc.addPrimitive(pC);
    sc.addPrimitive(pB);

    sc.addPrimitive(sL);
    sc.addPrimitive(sR);

    sc.addLight(light);

    return sc;
}


Scene cornellCSGs(const SceneProps& props) {

    Camera cam(
        Vector3(-1,0,0), Vector3(0, 1, 0), Vector3(0, 0, 3), Vector3(0, 0, -3.5), 
        props.viewportWidth, props.viewportHeight
    );

    Scene sc(props, cam);

    auto BRDFPL = make_shared<SimpleBRDF>(RGB(1, 0, 0));
    auto BRDFPR = make_shared<SimpleBRDF>(RGB(0, 1, 0));
    auto BRDFP = make_shared<SimpleBRDF>(RGB(0, 0, 1));

    auto BRDFL = make_shared<SimpleBRDF>(RGB(0.6, 0.75, 0.8));
    auto BRDFM = make_shared<SimpleBRDF>(RGB(0.8, 0.5, 0.3));
    auto BRDFR = make_shared<SimpleBRDF>(RGB(0.6, 0.7, 0.2));
    auto gray = make_shared<SimpleBRDF>(RGB(0.7, 0.7, 0.7));
    auto pL = make_shared<Plane> (1, Vector3(1, 0, 0), BRDFPL);
    auto pR = make_shared<Plane> (1, Vector3(-1, 0, 0), BRDFPR);
    auto pF = make_shared<Plane> (1, Vector3(0, 1, 0), gray);
    auto pC = make_shared<Plane> (1, Vector3(0, -1, 0), gray);
    auto pB = make_shared<Plane> (1, Vector3(0, 0, -1), gray);

    auto s1 = make_shared<Sphere>(Vector3(-0.6, -0.3, 0.5), 0.2, gray);
    auto s2 = make_shared<Sphere>(Vector3(-0.6, -0.4, 0.5), 0.2, gray);
    auto s3 = make_shared<Sphere>(Vector3(0, -0.3, 0.4), 0.25, gray);
    auto s4 = make_shared<Sphere>(Vector3(0, -0.4, 0.4), 0.25, gray);
    auto s5 = make_shared<Sphere>(Vector3(0.6, -0.3, 0.35), 0.2, gray);
    auto s6 = make_shared<Sphere>(Vector3(0.6, -0.4, 0.4), 0.25, gray);

    auto sCSG1 = make_shared<CSG>(s1, s2, CSGOperation::Union, BRDFL);
    auto sCSG2 = make_shared<CSG>(s3, s4, CSGOperation::Intersection, BRDFM);
    auto sCSG3 = make_shared<CSG>(s6, s5, CSGOperation::Difference, BRDFR);

    auto sL = make_shared<Sphere>(Vector3(-0.5, -0.7, 0.25), 0.3, BRDFL);
    auto sR = make_shared<Sphere>(Vector3( 0.5, -0.7, -0.25), 0.3, BRDFR);
    auto light  = make_shared<PointLight>(Vector3(0.0, 0.5, 0), RGB(1,1,1));

    sc.addPrimitive(pL);
    sc.addPrimitive(pR);
    sc.addPrimitive(pF);
    sc.addPrimitive(pC);
    sc.addPrimitive(pB);

    //sc.addPrimitive(sL);
    //sc.addPrimitive(sR);

    sc.addPrimitive(sCSG1);
    sc.addPrimitive(sCSG2);
    sc.addPrimitive(sCSG3);

    sc.addLight(light);

    return sc;
}

Scene cornellBoxOriginal(const SceneProps& props) {

    Camera cam(
        Vector3(-1,0,0), Vector3(0, 1, 0), Vector3(0, 0, 3), Vector3(0, 0, -3.5), 
        props.viewportWidth, props.viewportHeight
    );

    Scene sc(props, cam);

    auto BRDFPL = make_shared<SimpleBRDF>(RGB(1, 0, 0));
    auto BRDFPR = make_shared<SimpleBRDF>(RGB(0, 1, 0));
    auto BRDFL = make_shared<SimpleBRDF>(RGB(0.6, 0.75, 0.8));
    auto BRDFR = make_shared<SimpleBRDF>(RGB(0.6, 0.7, 0.2));
    auto gray = make_shared<SimpleBRDF>(RGB(0.7, 0.7, 0.7));
    auto pL = make_shared<Plane> (1, Vector3(1, 0, 0), BRDFPL);
    auto pR = make_shared<Plane> (1, Vector3(-1, 0, 0), BRDFPR);
    auto pF = make_shared<Plane> (1, Vector3(0, 1, 0), gray);
    auto pC = make_shared<Plane> (1, Vector3(0, -1, 0), gray);
    auto pB = make_shared<Plane> (1, Vector3(0, 0, -1), gray);
    auto pBack = make_shared<Plane> (-4, Vector3(0, 0, 1), gray);

    auto rL = make_shared<Box>(Vector3(-0.7, -1, 0), Vector3(-0.3, 0.2, 0.4), BRDFL);
    auto rR = make_shared<Box>(Vector3(0.3, -1, -0.4), Vector3(0.7, -0.3, 0), BRDFR);
    auto light  = make_shared<PointLight>(Vector3(0.0, 0.5, 0), RGB(1,1,1));

    sc.addPrimitive(pL);
    sc.addPrimitive(pR);
    sc.addPrimitive(pF);
    sc.addPrimitive(pC);
    sc.addPrimitive(pB);

    sc.addPrimitive(rL);
    sc.addPrimitive(rR);

    sc.addLight(light);

    return sc;
}

Scene cornellBoxOriginal2(const SceneProps& props) {

    Camera cam(
        Vector3(-1,0,0), Vector3(0, 1, 0), Vector3(0, 0, 3), Vector3(0, 0, -3.5), 
        props.viewportWidth, props.viewportHeight
    );

    Scene sc(props, cam);

    auto emissionPink = make_shared<Emitter>(RGB(1, 0, 1));
    auto emissionCyan = make_shared<Emitter>(RGB(0, 1, 1));
    auto BRDFL = make_shared<SimpleBRDF>(RGB(0.6, 0.75, 0.8));
    auto BRDFR = make_shared<SimpleBRDF>(RGB(0.6, 0.7, 0.2));
    auto gray = make_shared<SimpleBRDF>(RGB(0.7, 0.7, 0.7));
    auto pL = make_shared<Plane> (2, Vector3(1, 0, 0), emissionPink);
    auto pR = make_shared<Plane> (2, Vector3(-1, 0, 0), emissionCyan);
    auto pF = make_shared<Plane> (1, Vector3(0, 1, 0), gray);
    auto pC = make_shared<Plane> (1, Vector3(0, -1, 0), gray);
    auto pB = make_shared<Plane> (1, Vector3(0, 0, -1), gray);
    auto pBack = make_shared<Plane> (-4, Vector3(0, 0, 1), gray);

    auto rL = make_shared<Box>(Vector3(-0.7, -1, 0), Vector3(-0.3, 0.2, 0.4), BRDFL);
    auto rR = make_shared<Box>(Vector3(0.3, -1, -0.4), Vector3(0.7, -0.3, 0), BRDFR);
    auto light  = make_shared<PointLight>(Vector3(0.0, 0.5, 0), RGB(1,1,1));

    sc.addPrimitive(pL);
    sc.addPrimitive(pR);
    sc.addPrimitive(pF);
    sc.addPrimitive(pC);
    sc.addPrimitive(pB);

    sc.addPrimitive(rL);
    sc.addPrimitive(rR);

    //sc.addLight(light);

    return sc;
}