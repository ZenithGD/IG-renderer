#include <scene/BSDF.hpp>
#include <math/coordinate.hpp>

#include <functional>
#include <random>

RGB BSDF::eval(const Vector3 x, const Vector3 omegaI, const Vector3 omega, const Vector3 n) const {
    
    Vector3 specDir = sampleSpecular(omega, x, n);
    Vector3 refDir = sampleRefraction(omega, x, n);
    
    RGB dif = probDiffuse > 0 ? diffuse / M_PI / probDiffuse : RGB();
    RGB spec = probSpecular > 0 ? specular * (delta(omegaI, specDir)) / probSpecular : RGB();
    RGB ref = probRefraction > 0 ? refraction * (delta(omegaI, refDir)) / probRefraction : RGB();
    
    return dif + spec + ref;
}

Vector3 BSDF::sampleDiffuse(const Vector3 omega0, const Vector3 x, const Vector3 n) const {
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

Vector3 BSDF::sampleSpecular(const Vector3 omega0, const Vector3 x, const Vector3 n) const {
    return omega0 - n * 2 * dot(omega0, n);
}


Vector3 BSDF::sampleRefraction(const Vector3 omega0, const Vector3 x, const Vector3 n) const {

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

BSDF::EventType BSDF::russianRoulette(double t) {
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

tuple<Vector3, RGB> BSDF::sample(const Vector3 omega0, const Vector3 x, const Vector3 n){
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
        sampleDir = sampleRefraction(omega0, x, n);
        break;
    default:
        return { Vector3(), RGB() };
    }

    return { sampleDir, eval(x, sampleDir, omega0, n) };
}
