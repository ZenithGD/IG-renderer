#include "PhongBRDF.hpp"
#include <math/coordinate.hpp>

#include <functional>
#include <random>

RGB PhongBRDF::eval(const Vector3 x, const Vector3 omegaI, const Vector3 omega, const Vector3 n) const {
    
    Vector3 specDir = sampleSpecular(omega, x, n);
    
    return diffuse / M_PI + specular * (alpha + 2) / ( 2 * M_PI ) * pow(abs(dot(omega, specDir)), alpha);
}

Vector3 PhongBRDF::sampleDiffuse(const Vector3 omega0, const Vector3 x, const Vector3 n) const {
    std::random_device rand_dev;
    uniform_real_distribution<double> distribution(0.0,1.0);
    default_random_engine generator(rand_dev());
    auto dice = std::bind ( distribution, generator );
    
    double invTheta = acos(sqrt(1.0 - dice()));
    double invPhi = 2.0 * M_PI * dice();

    Vector3 omega = Vector3(
        sin(invTheta) * cos(invPhi),
        sin(invTheta) * sin(invPhi),
        cos(invTheta)).normalized();

    Vector3 perp = perpendicular(n);
    Coordinate global2Local(cross(perp, n), perp, n, x, 1);
    Coordinate dir(Vector3(1,0,0), Vector3(0,1,0), Vector3(0,0,1), omega, 0);

    Coordinate local2Global = inverseTransformation(global2Local);

    return local2Global(dir).getPosition();
}

Vector3 PhongBRDF::sampleSpecular(const Vector3 omega0, const Vector3 x, const Vector3 n) const {
    return omega0 - n * 2 * dot(omega0, n);
}

PhongBRDF::EventType PhongBRDF::russianRoulette(double t) const {
    if ( t < probPhong ) {
        return PHONG;
    } else {
        return ABSORPTION;
    }
}

optional<tuple<Vector3, RGB>> PhongBRDF::sample(const Vector3 omega0, const Vector3 x, const Vector3 n) const{
    std::random_device rand_dev;
    uniform_real_distribution<double> distribution(0.0,1.0);
    default_random_engine generator(rand_dev());
    auto dice = std::bind ( distribution, generator );
    
    // Russian roulette
    double r = dice();
    Vector3 sampleDir;
    switch (russianRoulette(r))
    {
    case PHONG:
        sampleDir = sampleDiffuse(omega0, x, n);
        break;
    default:
        return {};
    }

    return make_optional<tuple<Vector3, RGB>>(sampleDir, eval(x, sampleDir, omega0, n));
}
