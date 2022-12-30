#pragma once

#include <math/vector3.hpp>

/**
 * @brief Perform barycentric interpolation of vectors v0, v1, v2 for 
 * barycentric coordinates u, v, w
 * 
 * @param v0 Vertex of the triangle
 * @param v1 Vertex of the triangle
 * @param v2 Vertex of the triangle
 * @param u Barycentric coordinate
 * @param v Barycentric coordinate
 * @return Vector3 The interpolated vector
 */
template <typename V>
V barInterpolation(const V& v0, const V& v1, const V& v2, double u, double v) {
    double w = 1 - u - v;

    return v0 * u + v1 * v + v2 * w;
}

