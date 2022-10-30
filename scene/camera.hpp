#pragma once 

#include <list>

#include <math/vector3.hpp>
#include <geometry/ray.hpp>
#include <random>

class Camera {
public:
    Vector3 left, up, forward, origin;
    int width, height;

    Camera(Vector3 l, Vector3 u, Vector3 f, Vector3 o, unsigned int _w, unsigned int _h)
        : left(l), up(u), forward(f), origin(o), width(_w), height(_h) {}

    list<Ray> perPixel(unsigned int x, unsigned int y, unsigned int n);
};