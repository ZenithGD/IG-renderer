#pragma once

#include <math/vector3.hpp>
#include <color/color.hpp>
#include <cmath>
#include <tuple>

class BSDF {
public:
    RGB diffuse;
    BSDF(const RGB d = RGB()) : diffuse(d) {};

    RGB eval(const Vector3 x, const Vector3 dir1, const Vector3 omega) const;
    tuple<Vector3, RGB> sample(const double theta, const double phi, const Vector3 omega0, const Vector3 x, const Vector3 n);

};