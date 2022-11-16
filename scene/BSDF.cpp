#include <scene/BSDF.hpp>
#include <math/coordinate.hpp>

#include <functional>
#include <random>

RGB BSDF::eval(const Vector3 x, const Vector3 omegaI, const Vector3 omega, const Vector3 n, double index0) const {
    
    
    RGB dif = probDiffuse > 0 ? diffuse / M_PI / probDiffuse : RGB();
    RGB spec = probSpecular > 0 ? specular * (delta(omegaI, omega) / dot(n,omegaI)) / probSpecular : RGB();
    RGB ref = probRefraction > 0 ? refraction * (delta(omegaI,omega) / dot(n,omegaI)) / probRefraction : RGB();
    return dif + spec + ref;
}

Vector3 BSDF::sampleDiffuse(const Vector3 omega0, const Vector3 x, const Vector3 n) {
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

Vector3 BSDF::sampleSpecular(const Vector3 omega0, const Vector3 x, const Vector3 n){
    return omega0 - n * 2 * dot(omega0, n);
}

Vector3 BSDF::sampleRefraction(const Vector3 omega0, const Vector3 x, const Vector3 n, const double index0){
    double thetaI = asin(sin(angle(omega0,n)) * index0/refractionIndex);
    double seno = sin(thetaI);
    double coseno = cos(thetaI);

    Vector3 omegaI = Vector3(seno, coseno, 0.0);

    Vector3 perp = n.projection(omegaI) - omega0;
    Coordinate global2Local(cross(perp, n), perp, n, x, 1);
    Coordinate dir(Vector3(1,0,0), Vector3(0,1,0), Vector3(0,0,1), omega0, 0);

    Coordinate local2Global = inverseTransformation(global2Local);

    return local2Global(dir).getPosition();
}

BSDF::EventType BSDF::russianRoulette(double t) {
    if ( t < probDiffuse ) {
        return DIFFUSE;
    } else if ( t < probSpecular ) {
        return SPECULAR;
    } else if ( t < probRefraction ) {
        return REFRACTION;
    } else {
        return ABSORPTION;
    }
}

tuple<Vector3, RGB> BSDF::sample(const Vector3 omega0, const Vector3 x, const Vector3 n, double index0){
    std::random_device rand_dev;
    uniform_real_distribution<double> distribution(0.0,1.0);
    default_random_engine generator(rand_dev());
    auto dice = std::bind ( distribution, generator );
    
    // Russian roulette
    double r = dice();
    Vector3 sampleDir;
    switch (russianRoulette(r))
    {
    case DIFFUSE:
        sampleDir = sampleDiffuse(omega0, x, n);
        break;
    case SPECULAR:
        sampleDir = sampleSpecular(omega0, x, n);
        break;
    case REFRACTION:
        sampleDir = sampleRefraction(omega0, x, n, index0);
        break;
    default:
        return { Vector3(), RGB() };
    }

    return { sampleDir, eval(x, sampleDir, omega0, n, index0) };
}
