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

#include <set>


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

    auto img = Image::readPPM("assets/envmaps/moonlit.ppm");
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

shared_ptr<TriangleMesh> waterPlane(const Vector3& lb, const Vector3& hb,
    const unsigned int nx, const unsigned int nz, const double amp, const shared_ptr<BRDF>& brdf)
{
    FractalNoise fn(32, 6, abs(hb.x - lb.x), abs(hb.z - lb.z));

    vector<Vector3> points, normals;
    vector<Vector2> uvs;
    vector<unsigned int> indices;

    double stepX = (hb.x - lb.x) / (double)nx;
    double stepZ = (hb.z - lb.z) / (double)nz;

    for ( auto i = 0; i <= nx; i++ ) {
        for ( auto j = 0; j <= nz; j++ ) {

            double height = fn(lb.x + i * stepX, lb.z + j * stepZ) * amp;
            double hy = lb.y;
            if ( i != 0 && i != nx && j != 0 && j != nz ) {
                hy += height;
            }
            points.emplace_back(lb.x + i * stepX, hy, lb.z + j * stepZ);
            
            uvs.emplace_back(double(i) / double(nx), double(j) / double(nz));
        }
    } 

    // add triangles
    for ( auto i = 0; i < nx; i++ ) {
        for ( auto j = 0; j < nz; j++ ) {

            unsigned int i0 = (nz + 1) * i + j,
                         i1 = (nz + 1) * i + j + 1,
                         i2 = (nz + 1) * (i + 1) + j,
                         i3 = (nz + 1) * (i + 1) + j + 1;

            Vector3 p0 = points[i0],
                    p1 = points[i1], 
                    p2 = points[i2], 
                    p3 = points[i3];

            
            Vector3 n0 = cross(p1 - p0, p2 - p0).normalized();
            Vector3 n1 = cross(p2 - p3, p1 - p3).normalized();

            // first tri
            indices.push_back(i0);
            normals.push_back(n0);
            indices.push_back(i1);
            normals.push_back(n0);
            indices.push_back(i2);
            normals.push_back(n0);

            // second tri
            indices.push_back(i1);
            normals.push_back(n1);
            indices.push_back(i2);
            normals.push_back(n1);
            indices.push_back(i3);
            normals.push_back(n1);
        }
    }

    return make_shared<TriangleMesh>(points, indices, normals, uvs, brdf);
}

list<shared_ptr<Primitive>> seaBottom(const Vector3& lb, const Vector3& hb, 
    const double step, const double amp, const double pad) 
{
    FractalNoise fn(32, 5, abs(hb.x - lb.x), abs(hb.z - lb.z));

    list<shared_ptr<Primitive>> plane;
    
    auto gray = make_shared<SimpleBRDF>(RGB(1, 1, 1));

    for ( double sx = lb.x; sx < hb.x; sx += step ) {
        for ( double sz = lb.z; sz < hb.z; sz += step ) {
            double height = fn(sx - lb.x, sz - lb.z) * amp;
            plane.push_back(make_shared<Box>(Vector3(sx + pad, -300, sz + pad), Vector3(sx + step - pad, lb.y + height, sz + step - pad), gray));
        }
    }

    return plane;
}

list<shared_ptr<Primitive>> mengerSponge(const Vector3& lb, const Vector3& hb,
    const unsigned int it, const shared_ptr<BRDF>& brdf) 
{
    list<shared_ptr<Primitive>> r;
    if ( it > 0 ) {

        unsigned int skip[7] = {4, 10, 12, 13, 14, 16, 22};
        unsigned int skipPtr = 0;

        double width = hb.x - lb.x, height = hb.y - lb.y, depth = hb.z - lb.z; 
        for ( int i = 0; i < 3; i++ ) {
            for ( int j = 0; j < 3; j++ ) {
                for ( int k = 0; k < 3; k++ ) {
                    if ( skipPtr < 7 && skip[skipPtr] == (9 * i + 3 * j + k) ) {
                        skipPtr++;
                    } else {
                        Vector3 lbp = lb + Vector3(
                            width * (double)i / 3.0,
                            height * (double)j / 3.0,
                            depth * (double)k / 3.0);
                        Vector3 hbp = lbp + Vector3(width / 3.0, height / 3.0, depth / 3.0);
                        for ( auto& p : mengerSponge(lbp, hbp, it - 1, brdf) ) 
                            r.push_back(p);
                    }
                }
            }
        }
    } else {
        r.push_back(make_shared<Box>(lb, hb, brdf));
    }
    return r;
}

Scene renderScene(const SceneProps& props) {

    double scale = 100;
    Vector3 forward(0, -35, 300);
    Camera cam(
        Vector3(-scale,0,0), 
        cross(forward, Vector3(1, 0, 0)).normalized() * scale * (double)props.viewportHeight / (double)props.viewportWidth, 
        forward,
        Vector3(0, 100, -1000), 
        props.viewportWidth, props.viewportHeight
    );

    // env map
    auto img = tonemapping::simpleReinhard(Image::readPPM("assets/envmaps/milkyway.ppm"), 0.04, 0.0001);
    auto tex1 = make_shared<ImageTexture>(img);
    Scene sc(props, cam, tex1);

    // texture moon
    auto moonImg = Image::readPPM("assets/wmt.ppm");
    auto textMoon = make_shared<ImageTexture>(moonImg, 100, 100);

    //texture cyber 
    auto cyberImage = Image::readPPM("assets/tex/cyber.ppm");
    auto textCyber = make_shared<ImageTexture>(cyberImage);

    // some scene params
    double shoreLineZ = -50;
    double groundLevelY = -100;
    double waterBaseLevel = groundLevelY - 300;
    double spongeSize = 150;

    double lateralLightPower = 2;
    double moonPower = 1.2;

    auto reflection = make_shared<SimpleBRDF>(RGB(0.5,0.5,0.5), RGB(0.5, 0.5, 0.5));
    auto waterBRDF = make_shared<FresnelBRDF>(RGB(1, 1, 1), RGB(1, 1, 1), 1.333);
    auto crystalBRDF = make_shared<FresnelBRDF>(RGB(1, 1, 1), RGB(1, 1, 1), 1.52);
    auto emissionPink = make_shared<Emitter>(RGB(1, 0, 1) * lateralLightPower);
    auto emissionCyan = make_shared<Emitter>(RGB(0, 1, 1) * lateralLightPower);
    //auto moonLight = make_shared<TextureEmitter>(textMoon, 300, 300);
    auto moonLight = make_shared<Emitter>(RGB(0.92,0.95,0.96) * moonPower);

    auto noise = make_shared<FractalNoise>(16, 4, 1, 1);
    auto tNoise = make_shared<WoodyNoise>(noise, 5);
    auto tex = make_shared<NoiseTexture>(tNoise, RGB(0,0,0), RGB(1,1,1));
    auto noiseTexture = make_shared<TextureBRDF>(tex, 1);

    auto BRDFL = make_shared<SimpleBRDF>(RGB(0.6, 0.75, 0.8));
    auto BRDFR = make_shared<SimpleBRDF>(RGB(0.6, 0.7, 0.2));
    auto gray = make_shared<SimpleBRDF>(RGB(1, 1, 1));
    auto orange = make_shared<SimpleBRDF>(RGB(1,0.7,0.27));
    auto green = make_shared<SimpleBRDF>(RGB(0.74,0.925,0.714));
    auto blue = make_shared<SimpleBRDF>(RGB(0.502,0.8079,1));
    auto pL = make_shared<RectangleYZ>(-100, 600, -1000, 200, -400, emissionPink);
    auto pR = make_shared<RectangleYZ>(-100, 600, -1000, 200, 400, emissionCyan);
    auto cyberBRDF = make_shared<TextureBRDF>(textCyber,1);
    
    auto moonSphere1 = make_shared<Sphere>(Vector3(0, 400, 2000), 350, moonLight);
    auto moonSphere2 = make_shared<Sphere>(Vector3(-25, 375, 2000), 350, moonLight);
    auto moonSphere = make_shared<CSG>(moonSphere1, moonSphere2, CSGOperation::Difference, moonLight);

    auto ground = make_shared<Box>(Vector3(-1000, -400, shoreLineZ), Vector3(1000, groundLevelY, 1000), gray);
    auto rL = make_shared<Box>(Vector3(-70, -100, 0), Vector3(-30, 20, 40), BRDFL);
    auto rR = make_shared<Box>(Vector3(30, -100, -40), Vector3(70, -30, 0), BRDFR);
    //auto water = make_shared<Plane>(110, Vector3(0, 1, 0), waterBRDF);
    auto bottomLight = make_shared<Plane>(400, Vector3(0, 1, 0), emissionPink);

    auto boxSphere = make_shared<Box>(Vector3(-250,-100,-20), Vector3(-150, 20, 20), gray);
    auto sphereFresnel = make_shared<Sphere>(Vector3(-200,70,0), 50.0, crystalBRDF);
    auto sphereDifference1 = make_shared<Sphere>(Vector3(-200,-50,-20), 30.0, gray);
    auto sphereDifference2 = make_shared<Sphere>(Vector3(-200,-50,-30), 27.5, gray);
    auto sphereMini = make_shared<Sphere>(Vector3(-200,-50,-30), 10.0, gray);
    auto csgSphere = make_shared<CSG>(sphereDifference1, sphereDifference2, CSGOperation::Difference, gray);

    auto csg1 = make_shared<CSG>(boxSphere, csgSphere, CSGOperation::Union, gray);
    auto speaker = make_shared<CSG>(csg1, sphereMini, CSGOperation::Union, gray);
    auto otherBox = make_shared<Box>(Vector3(-190,-100,40), Vector3(-80,0,70), orange);
    
    auto leftBox1 = make_shared<Box>(Vector3(60, groundLevelY, 50), Vector3(160, 60, 200), noiseTexture);
    auto leftBox2 = make_shared<Box>(Vector3(100, groundLevelY, 150), Vector3(200, 100, 200), green);

    auto behindBox = make_shared<Box>(Vector3(30, groundLevelY, 200), Vector3(70, 70, 250), blue);
    auto higherBox = make_shared<Box>(Vector3(-250, groundLevelY, 300), Vector3(-150, 200, 400), cyberBRDF);
    // left emission planes
    sc.addPrimitive(pL);
    sc.addPrimitive(pR);
    
    //sc.addPrimitive(rL);
    //sc.addPrimitive(rR);
    sc.addPrimitive(moonSphere1);
    
    sc.addPrimitive(ground);
    //sc.addPrimitive(bottomLight);

    for ( auto& p : seaBottom(Vector3(-400, waterBaseLevel, shoreLineZ - 400), Vector3(400, waterBaseLevel, shoreLineZ), 200, 360, 1) )
        sc.addPrimitive(p);

    for ( auto& p : mengerSponge(Vector3(100, groundLevelY, 0), Vector3(100 + spongeSize, groundLevelY + spongeSize, spongeSize), 2, reflection) )
        sc.addPrimitive(p);

    //sc.addLight(light);
    //sc.addPrimitive(water);
    // original 40 x 20 grid
    sc.addPrimitive(waterPlane(Vector3(-400, groundLevelY - 50, -500), Vector3(400, groundLevelY - 50, shoreLineZ), 4, 3, 50, waterBRDF));
    
    sc.addPrimitive(waterPlane(Vector3(-1000, groundLevelY, 500), Vector3(1000, groundLevelY, 1000), 8, 3, 400, gray));
    
    sc.addPrimitive(boxSphere);
    sc.addPrimitive(sphereFresnel);
    sc.addPrimitive(speaker);
    sc.addPrimitive(otherBox);
    sc.addPrimitive(leftBox1);
    sc.addPrimitive(leftBox2);
    sc.addPrimitive(behindBox);
    sc.addPrimitive(higherBox);

    return sc;
}