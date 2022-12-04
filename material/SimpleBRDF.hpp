#pragma once

#include <material/BRDF.hpp>

#include <math/vector3.hpp>
#include <color/color.hpp>

#include <cmath>
#include <tuple>

class SimpleBRDF : public BRDF{
public:
    RGB diffuse, specular, refraction;
    double probDiffuse, probSpecular, probRefraction;

    double refractionIndex;

    /**
     * @brief Construct a new BRDF object and assign the probability 
     * for each event after a ray bounce automatically.
     * 
     * The probability for any given event will be given by the maximum value
     * of its colour channels.
     * 
     * @param d The diffuse coefficient
     * @param s The specular coefficient
     * @param r The refractive coefficient
     * @param ri The refraction index 
     */
    SimpleBRDF(const RGB& d = RGB(), const RGB& s = RGB(), const RGB& r = RGB(), const double ri = 1) 
        : BRDF(false),
          diffuse(d), 
          specular(s), 
          refraction(r),
          probDiffuse(maxChannel(diffuse)), 
          probSpecular(maxChannel(specular)), 
          probRefraction(maxChannel(refraction)), 
          refractionIndex(ri) 
    {};

    /**
     * @brief Construct a new BRDF object and manually assign event probabilities
     * 
     * @param d The diffuse coefficient
     * @param s The specular coefficient
     * @param r The refractive coefficient
     * @param pd The probability of a diffuse event on a ray bounce
     * @param ps The probability of a specular event on a ray bounce
     * @param pr The probability of a refraction event on a ray bounce
     * @param ri The refraction index 
     */
    SimpleBRDF(const RGB& d, const RGB& s, const RGB& r, 
         const double pd, const double ps, const double pr, const double ri) 
        : BRDF(false),
          diffuse(d), 
          specular(s), 
          refraction(r),
          probDiffuse(pd), 
          probSpecular(ps), 
          probRefraction(pr), 
          refractionIndex(ri) 
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
     * @return tuple<Vector3, RGB> 
     */
    optional<tuple<Vector3, RGB>> sample(const Vector3& omega0, const Vector3& x, const Intersection& it) const override;

private:

    /**
     * @brief An enum for listing event types for the Russian roulette algorithm.
     * 
     * - Diffuse events scatter light in every direction.
     * - Reflection events only scatter light following the reflection law.
     * - Refraction events only scatter light following Snell's law:
     * 
     *  $ n_1 \cdot sin \theta_1 = n_2 \cdot sin \theta_2 $
     */
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
    Vector3 sampleDiffuse(const Vector3& omega0, const Vector3& x, const Vector3& n) const;
    
    /**
     * @brief Sample an outbound direction for a reflective surface
     * 
     * @param omega0 The inbound direction
     * @param x The intersection point
     * @param n The normal
     * @return Vector3 
     */
    Vector3 sampleSpecular(const Vector3& omega0, const Vector3& x, const Vector3& n) const;
    
    /**
     * @brief Sample an outbound direction for a refractive surface
     * 
     * @param omega0 The inbound direction
     * @param x The intersection point
     * @param n The normal
     * @return Vector3 
     */
    Vector3 sampleRefraction(const Vector3& omega0, const Vector3& x, const Vector3& n) const;
    
    /**
     * @brief Return the event type given a random value between 0 and 1.
     * 
     * @param t A value between 0 and 1
     * @return EventType The event type
     */
    EventType russianRoulette(double t) const;
};