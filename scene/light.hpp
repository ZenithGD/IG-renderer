#pragma once 
#include <math/vector3.hpp>
#include <color/color.hpp>

using namespace std;

class Light {
public:
    RGB power;

    Light(RGB p) : power(p) {}

    virtual ~Light() = default;
};

class PointLight : public Light {
public:
    Vector3 center;

    PointLight(Vector3 c, RGB p) : Light(p), center(c) {}
};

class AreaLight : public Light {
public:
    
};