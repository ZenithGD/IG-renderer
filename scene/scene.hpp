#pragma once

#include <vector>
#include <memory>

#include <geometry/primitive.hpp>
#include <image/image.hpp>
#include <scene/camera.hpp>

using namespace std;

struct SceneProps {
    uint32_t viewportWidth, viewportHeight;
    uint32_t antialiasingFactor;
};

class Scene {
public:

    Scene(SceneProps s, Camera c) : cam(c), _scprops(s) {};

    Camera cam;

    // TODO change to acceleration data structure
    vector<shared_ptr<Primitive>> _primitives;

    void addPrimitive(const shared_ptr<Primitive> p);

    Image drawScene();

private:
    SceneProps _scprops;
};