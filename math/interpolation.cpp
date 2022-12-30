#include "interpolation.hpp"

Vector3 barInterpolation(
    const Vector3& v0, const Vector3& v1, const Vector3& v2,
    double u, double v) {

    double w = 1 - u - v;

    return v0 * u + v1 * v + v2 * w;
}