#pragma once 
#include <math/vector3.hpp>
#include <color/color.hpp>
#include <geometry/ray.hpp>

using namespace std;

class Light {
public:
    RGB power;

    Light(RGB p) : power(p) {}

    virtual Ray sample() const = 0;

    virtual ~Light() = default;
};

class PointLight : public Light {
public:
    Vector3 center;

    PointLight(Vector3 c, RGB p) : Light(p), center(c) {}

    Ray sample() const override;
};

class AreaLight : public Light {
public:

    Ray sample() const override;
    
};