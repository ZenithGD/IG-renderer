#pragma once

#include <vector>
#include <memory>

#include <geometry/primitive.hpp>
#include <image/image.hpp>
#include <scene/camera.hpp>

using namespace std;

class Scene {
public:

    Scene(Camera c) : cam(c) {};

    Camera cam;
    vector<shared_ptr<Primitive>> _primitives;

    void addPrimitive(const shared_ptr<Primitive> p);

    Image drawScene();
};