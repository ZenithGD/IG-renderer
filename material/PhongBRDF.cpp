#include "PhongBRDF.hpp"
#include <math/coordinate.hpp>

#include <functional>
#include <random>

RGB PhongBRDF::eval(const Vector3& x, const Vector3& omegaI, const Vector3& omega, const Intersection& it) const {
    
    Vector3 specDir = sampleSpecular(omega, x, it.closestNormal());
    
    return diffuse / M_PI + specular * (alpha + 2) / ( 2 * M_PI ) * pow(abs(dot(omega, specDir)), alpha);
}

Vector3 PhongBRDF::sampleDiffuse(const Vector3& omega0, const Vector3& x, const Vector3& n) const {
    RandomGenerator rng(0, 1);
    
    double invTheta = acos(sqrt(1.0 - rng()));
    double invPhi = 2.0 * M_PI * rng();

    Vector3 omega = Vector3(
        sin(invTheta) * cos(invPhi),
        sin(invTheta) * sin(invPhi),
        cos(invTheta)).normalized();

    Vector3 v1 = perpendicular(n);
    Vector3 v2 = cross(v1, n);
    Vector3 v3 = cross(v2, n);
    Coordinate local2Global(v2, v3, n, x, 1);
    Coordinate dir(Vector3(1,0,0), Vector3(0,1,0), Vector3(0,0,1), omega, 0);

    return local2Global(dir).getPosition();
}

Vector3 PhongBRDF::sampleSpecular(const Vector3& omega0, const Vector3& x, const Vector3& n) const {
    return omega0 - n * 2 * dot(omega0, n);
}

PhongBRDF::EventType PhongBRDF::russianRoulette(double t) const {
    if ( t < probPhong ) {
        return PHONG;
    } else {
        return ABSORPTION;
    }
}

optional<BRDFInteraction> PhongBRDF::sample(const Vector3& omega0, const Vector3& x, const Intersection& it) const{
    RandomGenerator rng(0, 1);
    
    // Russian roulette
    double r = rng();
    Vector3 sampleDir;
    switch (russianRoulette(r))
    {
    case PHONG:
        sampleDir = sampleDiffuse(omega0, x, it.closestNormal());
        break;
    default:
        return {};
    }

    return make_optional<BRDFInteraction>(sampleDir, eval(x, sampleDir, omega0, it), false);
}
