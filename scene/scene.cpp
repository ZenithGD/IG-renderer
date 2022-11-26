#include "scene.hpp"
#include <cmath>
#include <functional>
#include <random>
#include <iomanip>

#include <acceleration/threadpool.hpp>
#include <scene/BSDF.hpp>

void Scene::addPrimitive(const shared_ptr<Primitive> p)
{
    primitives.push_back(p);
}

void Scene::addLight(const shared_ptr<Light> l)
{
    lights.push_back(l);
}
