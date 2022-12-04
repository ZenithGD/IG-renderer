#include "FresnelBRDF.hpp"
#include <math/coordinate.hpp>

#include <functional>
#include <random>

RGB FresnelBRDF::eval(const Vector3& x, const Vector3& omegaI, const Vector3& omega, const Intersection& it) const {
    
    Vector3 n = it.closestNormal();
    Vector3 specDir = sampleSpecular(omega, x, n);
    Vector3 refDir = sampleRefraction(omega, x, n);

    bool frontFace = dot(omega, n) < 0; 
    double sqratio = frontFace ? 1 / ( refractionIndex * refractionIndex ) : ( refractionIndex * refractionIndex );

    RGB spec = specular * delta(omegaI, specDir);
    RGB ref = refraction * sqratio * delta(omegaI, refDir);
    
    return spec + ref;
}

Vector3 FresnelBRDF::sampleSpecular(const Vector3& omega0, const Vector3& x, const Vector3& n) const {
    return omega0 - n * 2 * dot(omega0, n);
}


Vector3 FresnelBRDF::sampleRefraction(const Vector3& omega0, const Vector3& x, const Vector3& n) const {

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

FresnelBRDF::EventType FresnelBRDF::russianRoulette(double t, double prefl) const {
    if ( t < prefl ) {
        return SPECULAR;
    } else {
        return REFRACTION;
    }
}

double fresnelCoef(double r0, double r1, double thi, double tht) {
    double par = (r1 * cos(tht) - r0 * cos(thi)) / (r1 * cos(tht) + r0 * cos(thi));
    double perp = (r0 * cos(tht) - r1 * cos(thi)) / (r0 * cos(tht) + r1 * cos(thi));

    return ( par * par + perp * perp ) / 2;
}

optional<tuple<Vector3, RGB>> FresnelBRDF::sample(const Vector3& omega0, const Vector3& x, const Intersection& it) const{
    Vector3 n = it.closestNormal();
    std::random_device rand_dev;
    uniform_real_distribution<double> distribution(0.0,1.0);
    default_random_engine generator(rand_dev());
    auto dice = std::bind ( distribution, generator );
    
    // Compute reflection probability based on fresnel coefficients
    bool frontFace = dot(omega0, n) < 0; 
    Vector3 ax = frontFace ? n : -n ;
    double r0 = frontFace ? 1 : refractionIndex,
           r1 = frontFace ? refractionIndex : 1;
    Vector3 specDir = sampleSpecular(omega0, x, n),
            refDir  = sampleRefraction(omega0, x, n);

    // Russian roulette
    double r = dice();
    Vector3 sampleDir;
    switch (russianRoulette(r, fresnelCoef(r0, r1, angle(ax, specDir), angle(ax, refDir))))
    {
    case SPECULAR:
        sampleDir = sampleSpecular(omega0, x, n);
        break;
    case REFRACTION:
        sampleDir = sampleRefraction(omega0, x, n);
        break;
    default:
        return {};
    }

    return make_optional<tuple<Vector3, RGB>>(sampleDir, eval(x, sampleDir, omega0, it));
}
