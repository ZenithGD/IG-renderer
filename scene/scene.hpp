#pragma once

#include <vector>
#include <memory>

#include <geometry/primitive.hpp>
#include <image/image.hpp>
#include <scene/camera.hpp>
#include <scene/light.hpp>

using namespace std;

struct SceneProps {
    uint32_t viewportWidth, viewportHeight;
    uint32_t antialiasingFactor;
    uint32_t threads;
};

class Scene {
public:

    Scene(SceneProps s, Camera c) : cam(c), _scprops(s) {};

    Camera cam;

    // TODO change to acceleration data structure
    vector<shared_ptr<Primitive>> _primitives;
    vector<shared_ptr<Light>> _lights;

    void addPrimitive(const shared_ptr<Primitive> p);
    void addLight(const shared_ptr<Light> l);

    Image drawScene();

private:

    RGB directLight(const Intersection it, const Vector3 obsDirection) const;
    SceneProps _scprops;
};