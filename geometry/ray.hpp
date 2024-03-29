#pragma once

#include <iostream>
#include <memory>

#include <math/vector3.hpp>

/**
 * @brief Class for representing Ray
 * 
 */
class Ray {
public:

    /**
     * @brief Construct a new Ray with values
     * @param o The value of the origin of the ray
     * @param d The value of the direction of the ray
     */
    Ray(Vector3 o, Vector3 d) : origin(o), direction(d) {}
    Vector3 origin, direction;

    /**
     * @brief Evaluate ray for value t.
     * 
     * @param t The value or the parameter.
     * @return Vector3 The result of the parametric equation v = o + t * d.
     */
    inline Vector3 eval(double t) const { return origin + direction * t; }

    inline Vector3 operator()(double t) const { return eval(t); }

    friend ostream& operator<<(ostream& os, const Ray& r);
};

Vector3 setFaceNormal(const Vector3& rdir, const Vector3& n);

ostream& operator<<(ostream& os, const Ray& r);