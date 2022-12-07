#include <scene/light.hpp>
#include <random>
#include <functional>

Ray PointLight::sample() const {
    //TODO: add sampling distribution for each light type
    RandomGenerator rng(0, 1);
    double invTheta = acos(2 * rng() - 1);
    double invPhi = 2.0 * M_PI * rng();

    Vector3 omega = Vector3(
        sin(invTheta) * cos(invPhi),
        sin(invTheta) * sin(invPhi),
        cos(invTheta)).normalized();

    return Ray(center, omega);
}