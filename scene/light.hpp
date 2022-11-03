#pragma once 
#include <math/vector3.hpp>
#include <color/color.hpp>

using namespace std;

class Light {
public:
    Vector3 center;
    RGB power;

    Light(Vector3 c, RGB p) : center(c), power(p) {}

};