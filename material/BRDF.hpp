#pragma once

#include <math/vector3.hpp>
#include <color/color.hpp>
#include <geometry/intersection.hpp>

#include <tuple>
#include <functional>

// Forward declaration of Intersection because of circular includes.
struct Intersection;

struct BRDFInteraction {
    Vector3 outDirection;
    RGB radiance;
    bool isDelta;

    BRDFInteraction(const Vector3& o, const RGB& r, bool delta) 
    : outDirection(o), radiance(r), isDelta(delta) {}
};

class BRDF {
public:
    bool emitter;

    /**
     * @brief Evaluate the BRDF on a point based on input and output directions
     * 
     * @param x The point in which the BRDF is evaluated
     * @param omegaI The input ray's direction
     * @param omega The output ray's direction
     * @param n The normal on the surface in which x lies
     * @return RGB The BRDF's result
     */
    virtual RGB eval(const Vector3& x, const Vector3& omegaI, 
        const Vector3& omega, const Intersection& it) const = 0;

    /**
     * @brief Sample the BRDF, return an outbound ray direction and the BRDF's value
     * 
     * @param omega0 The input ray's direction
     * @param x The point in which the BRDF is evaluated
     * @param n The normal on the surface in which x lies
     * @return tuple<Vector3, RGB> 
     */
    virtual optional<BRDFInteraction> sample(const Vector3& omega0, 
        const Vector3& x, const Intersection& it) const = 0;

    BRDF(bool emits) : emitter(emits) {}
};