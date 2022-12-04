#pragma once

#include <material/BRDF.hpp>

#include <math/vector3.hpp>
#include <color/color.hpp>

#include <cmath>
#include <tuple>

class Emitter : public BRDF{
public:
    RGB emission;

    /**
     * @brief Construct a new Emitter object
     * 
     * @param e Emission value
     */
    Emitter(const RGB e = RGB())
        : BRDF(true),
          emission(e)
        {};

    /**
     * @brief Evaluate the BRDF on a point based on input and output directions
     * 
     * @param x The point in which the BRDF is evaluated
     * @param omegaI The input ray's direction
     * @param omega The output ray's direction
     * @param n The normal on the surface in which x lies
     * @return RGB The BRDF's result
     */
    RGB eval(const Vector3& x, const Vector3& omegaI, const Vector3& omega, const Intersection& it) const override;
    
    /**
     * @brief Sample the BRDF, return an outbound ray direction and the BRDF's value
     * 
     * @param omega0 The input ray's direction
     * @param x The point in which the BRDF is evaluated
     * @param n The normal on the surface in which x lies
     * @return tuple
     */
    optional<tuple<Vector3, RGB>> sample(const Vector3& omega0, const Vector3& x, const Intersection& it) const override;
};