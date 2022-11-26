#include <scene/light.hpp>
#include <random>
#include <functional>

Ray PointLight::sample() const {
    //TODO: add sampling distribution for each light type
    std::random_device rand_dev;
    uniform_real_distribution<double> distribution(0.0,1.0);
    default_random_engine generator(rand_dev());
    auto dice = std::bind ( distribution, generator );
    double invTheta = acos(2 * dice() - 1);
    double invPhi = 2.0 * M_PI * dice();

    Vector3 omega = Vector3(
        sin(invTheta) * cos(invPhi),
        sin(invTheta) * sin(invPhi),
        cos(invTheta)).normalized();

    return Ray(center, omega);
}