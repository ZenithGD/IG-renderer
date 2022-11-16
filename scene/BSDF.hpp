#pragma once

#include <math/vector3.hpp>
#include <color/color.hpp>

#include <cmath>
#include <tuple>

class BSDF {
public:
    RGB diffuse, specular, refraction;
    double probDiffuse, probSpecular, probRefraction;

    double refractionIndex;

    BSDF(const RGB d = RGB(), const RGB s = RGB(), const RGB r = RGB(), const double ri = 1) 
        : diffuse(d), 
          specular(s), 
          refraction(r),
          probDiffuse(maxChannel(diffuse)), 
          probSpecular(maxChannel(specular)), 
          probRefraction(maxChannel(refraction)), 
          refractionIndex(ri) {};

    RGB eval(const Vector3 x, const Vector3 omegaI, const Vector3 omega, const Vector3 n, double index0) const;
    tuple<Vector3, RGB> sample(const Vector3 omega0, const Vector3 x, const Vector3 n, double index0);

private:

    enum EventType {
        DIFFUSE = 0,
        SPECULAR,
        REFRACTION,
        ABSORPTION
    };

    /**
     * @brief Sample an outbound direction for a diffuse surface
     * 
     * @param omega0 The inbound direction
     * @param x The intersection point
     * @param n The normal
     * @return Vector3 
     */
    Vector3 sampleDiffuse(const Vector3 omega0, const Vector3 x, const Vector3 n);
    Vector3 sampleSpecular(const Vector3 omega0, const Vector3 x, const Vector3 n);
    Vector3 sampleRefraction(const Vector3 omega0, const Vector3 x, const Vector3 n, const double index0);
    
    
    EventType russianRoulette(double t);
};