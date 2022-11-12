#include <scene/BSDF.hpp>

RGB BSDF::eval(const Vector3 x, const Vector3 dir1, const Vector3 omega) const {
    return diffuse / M_PI;
}

tuple<Vector3, RGB> BSDF::sample(const double theta, const double phi, const Vector3 omega0, const Vector3 x, const Vector3 n){
    double invTheta = acos(theta);
    double invPhi = 2 * M_PI * phi;

    Vector3 omega = Vector3(
        sin(invTheta) * cos(invPhi),
        sin(invTheta) * sin(invPhi),
        cos(invTheta)).normalized();

    return { omega, eval(x, omega, omega0) };
}