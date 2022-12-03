#pragma once

#include <math/vector3.hpp>
#include <color/color.hpp>
#include <tuple>
#include <functional>

class BRDF {
public:
    /**
     * @brief Evaluate the BRDF on a point based on input and output directions
     * 
     * @param x The point in which the BRDF is evaluated
     * @param omegaI The input ray's direction
     * @param omega The output ray's direction
     * @param n The normal on the surface in which x lies
     * @return RGB The BRDF's result
     */
    virtual RGB eval(const Vector3 x, const Vector3 omegaI, 
        const Vector3 omega, const Vector3 n) const = 0;

    /**
     * @brief Sample the BRDF, return an outbound ray direction and the BRDF's value
     * 
     * @param omega0 The input ray's direction
     * @param x The point in which the BRDF is evaluated
     * @param n The normal on the surface in which x lies
     * @return tuple<Vector3, RGB> 
     */
    virtual optional<tuple<Vector3, RGB>> sample(const Vector3 omega0, 
        const Vector3 x, const Vector3 n) const = 0;
};