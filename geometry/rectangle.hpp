#pragma once

#include <geometry/primitive.hpp>
#include <math/vector3.hpp>
#include <math/vector2.hpp>
#include <memory>

using namespace std;

/**
 * @brief XZ plane-aligned rectangle.
 * 
 */
class Rectangle : public Primitive {
public:
    Rectangle(double w, double h, const Vector3& o, const shared_ptr<BRDF>& brdf);

    Intersection intersection(const Ray& r, double minT, double maxT) override;

private:
    Vector2 getUVCoords(const Vector3& p);

    double x0, x1, z0, z1, k;
};
