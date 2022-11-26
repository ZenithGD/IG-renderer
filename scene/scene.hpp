#pragma once

#include <vector>
#include <memory>
#include <functional>

#include <geometry/primitive.hpp>
#include <image/image.hpp>
#include <scene/camera.hpp>
#include <scene/light.hpp>

using namespace std;

struct SceneProps {
    uint32_t viewportWidth, viewportHeight;
    uint32_t antialiasingFactor;
    uint32_t threads;
    
    unsigned int bounces = 5;
};

class Scene {
public:
    using RenderMethod = function<Image(const Scene&)>;

    Scene(SceneProps s, Camera c) : cam(c), _scprops(s) {};

    Camera cam;

    // TODO change to acceleration data structure
    vector<shared_ptr<Primitive>> primitives;
    vector<shared_ptr<Light>> lights;

    void addPrimitive(const shared_ptr<Primitive> p);
    void addLight(const shared_ptr<Light> l);

    inline SceneProps getProps() const { return _scprops; } 

    inline Image drawScene(RenderMethod method) { return method(*this); }
private:
    SceneProps _scprops;
};