#pragma once

#include <math/vector3.hpp>
#include <color/color.hpp>

/**
 * @brief Linear interpolation of lo and hi by t, where t \in [0, 1]
 * 
 * @tparam T 
 * @param lo The lower bound
 * @param hi The higher bound 
 * @param t The interpolation parameter
 * @return T The interpolated result
 */
template <typename T = double>
inline T lerp(const T& lo, const T& hi, const T& t) {
    return t * hi + (1 - t) * lo;
}

/**
 * @brief Evaluate the smoothstep function
 * 
 * @tparam T 
 * @param t The variable's value
 * @return T The value of the smoothstep function at t
 */
template <typename T = double>
inline T smoothstep(const T &t) { return t * t * (3 - 2 * t); }

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

/**
 * @brief Interpolate each colour channel and return the result.
 * 
 * @param ca The first colour.
 * @param cb The second colour.
 * @param t The interpolation parameter
 * @return RGB 
 */
RGB colorLerp(const RGB& ca, const RGB& cb, double t);

