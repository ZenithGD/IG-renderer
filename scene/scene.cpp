#include "scene.hpp"
#include <cmath>
#include <functional>
#include <random>
#include <iomanip>

#include <acceleration/threadpool.hpp>
#include <material/BRDF.hpp>

void Scene::addPrimitive(const shared_ptr<Primitive> p)
{
    primitives.push_back(p);
}

void Scene::addLight(const shared_ptr<Light> l)
{
    lights.push_back(l);
}

RGB Scene::environment(const Vector3& direction) const {

    if ( !_envMap ) return RGB();
    
    double phi = atan2(direction.z, -direction.x) + M_PI;
    double th  = acos(direction.y);

    return _envMap->sample(phi / (2 * M_PI), th / M_PI, Vector3());
}