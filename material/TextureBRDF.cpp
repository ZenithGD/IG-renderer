#include "TextureBRDF.hpp"

#include <math/coordinate.hpp>

#include <functional>
#include <random>

RGB TextureBRDF::eval(const Vector3& x, const Vector3& omegaI, const Vector3& omega, const Intersection& it) const {
    Vector3 n = it.closestNormal();
    Vector3 specDir = sampleSpecular(omega, x, n);
    Vector3 refDir = sampleRefraction(omega, x, n);
    
    RGB dif = probDiffuse > 0 ? albedo->sample(it.u, it.v, x) / M_PI: RGB();
    RGB spec = probSpecular > 0 ? specular * (delta(omegaI, specDir)) / dot(n, omegaI) : RGB();
    RGB ref = probRefraction > 0 ? refraction * (delta(omegaI, refDir))  / dot(n, omegaI) : RGB();
    
    return dif + spec + ref;
}

Vector3 TextureBRDF::sampleDiffuse(const Vector3& omega0, const Vector3& x, const Vector3& n) const {
    RandomGenerator rng(0, 1);
    
    double invTheta = acos(sqrt(1.0 - rng()));
    double invPhi = 2.0 * M_PI * rng();

    Vector3 omega = Vector3(
        sin(invTheta) * cos(invPhi),
        sin(invTheta) * sin(invPhi),
        cos(invTheta)).normalized();

    Vector3 perp = perpendicular(n);
    Coordinate global2Local(perp, cross(perp, n), n, x, 1);
    Coordinate dir(Vector3(1, 0, 0), Vector3(0,1,0), Vector3(0,0,1), omega, 0);

    Coordinate local2Global = inverseTransformation(global2Local);

    return local2Global(dir).getPosition();
}

Vector3 TextureBRDF::sampleSpecular(const Vector3& omega0, const Vector3& x, const Vector3& n) const {
    return omega0 - n * 2 * dot(omega0, n);
}


Vector3 TextureBRDF::sampleRefraction(const Vector3& omega0, const Vector3& x, const Vector3& n) const {

    double cosTh = min(dot(-omega0, n), 1.0);
    double sinTh = sqrt(1.0 - cosTh*cosTh);
    bool frontFace = dot(omega0, n) < 0; 

    Vector3 ax = frontFace ? n : -n;

    // Assume objects are solid and don't clip
    double ratio = frontFace ? 1.0 / refractionIndex : refractionIndex;

    bool cannotRefract = ratio * sinTh > 1.0;
    if ( cannotRefract ) {
        return sampleSpecular(omega0, x, ax);
    } else {
        auto cos = min(dot(-omega0, ax), 1.0);

        // Compute refracted parallel and perpendicular component to normal
        Vector3 rPerp = (omega0 + ax * cos) * ratio;
        Vector3 rPar = -ax * sqrt(abs(1.0 - dot(rPerp, rPerp)));
        
        // Sum both components together
        return rPerp + rPar;
    }
}

TextureBRDF::EventType TextureBRDF::russianRoulette(double t) const {
    if ( t < probDiffuse ) {
        return DIFFUSE;
    } else if ( t < probDiffuse + probSpecular ) {
        return SPECULAR;
    } else if ( t < probDiffuse + probSpecular + probRefraction ) {
        return REFRACTION;
    } else {
        return ABSORPTION;
    }
}

optional<BRDFInteraction> TextureBRDF::sample(const Vector3& omega0, const Vector3& x, const Intersection& it) const{
    Vector3 n = it.closestNormal();
    RandomGenerator rng(0, 1);
    
    // Russian roulette
    double r = rng();
    Vector3 sampleDir;
    bool isDelta;
    switch (russianRoulette(r))
    {
    case DIFFUSE:
        sampleDir = sampleDiffuse(omega0, x, n);
        isDelta = false;
        break;
    case SPECULAR:
        sampleDir = sampleSpecular(omega0, x, n);
        isDelta = true;
        break;
    case REFRACTION:
        sampleDir = sampleRefraction(omega0, x, n);
        isDelta = true;
        break;
    default:
        return {};
    }

    return make_optional<BRDFInteraction>(sampleDir, eval(x, sampleDir, omega0, it), isDelta);
}
