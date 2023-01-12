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
class RectangleXZ : public Primitive {
public:
    RectangleXZ(double l, double r, double b, double t, double _k, const shared_ptr<BRDF>& brdf)
        : Primitive(brdf), x0(l), x1(r), z0(b), z1(t), k(_k) {}

    RectangleXZ(double w, double h, const Vector3& o, const shared_ptr<BRDF>& brdf);

    Intersection intersection(const Ray& r, double minT, double maxT) override;

private:
    Vector2 getUVCoords(const Vector3& p);

    double x0, x1, z0, z1, k;
};

/**
 * @brief XY plane-aligned rectangle.
 * 
 */
class RectangleXY : public Primitive {
public:
    RectangleXY(double l, double r, double b, double t, double _k, const shared_ptr<BRDF>& brdf)
        : Primitive(brdf), x0(l), x1(r), y0(b), y1(t), k(_k) {}

    RectangleXY(double w, double h, const Vector3& o, const shared_ptr<BRDF>& brdf);

    Intersection intersection(const Ray& r, double minT, double maxT) override;

private:
    Vector2 getUVCoords(const Vector3& p);

    double x0, x1, y0, y1, k;
};

/**
 * @brief YZ plane-aligned rectangle.
 * 
 */
class RectangleYZ : public Primitive {
public:
    RectangleYZ(double l, double r, double b, double t, double _k, const shared_ptr<BRDF>& brdf)
        : Primitive(brdf), y0(l), y1(r), z0(b), z1(t), k(_k) {}
    RectangleYZ(double w, double h, const Vector3& o, const shared_ptr<BRDF>& brdf);

    Intersection intersection(const Ray& r, double minT, double maxT) override;

private:
    Vector2 getUVCoords(const Vector3& p);

    double y0, y1, z0, z1, k;
};

