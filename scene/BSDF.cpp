#include <scene/BSDF.hpp>
#include <math/coordinate.hpp>

RGB BSDF::eval(const Vector3 x, const Vector3 dir1, const Vector3 omega) const {
    return diffuse / M_PI;
}

tuple<Vector3, RGB> BSDF::sample(const double theta, const double phi, const Vector3 omega0, const Vector3 x, const Vector3 n){
    double invTheta = acos(sqrt(1.0 - theta));
    double invPhi = 2.0 * M_PI * phi;

    Vector3 omega = Vector3(
        sin(invTheta) * cos(invPhi),
        sin(invTheta) * sin(invPhi),
        cos(invTheta)).normalized();

    Vector3 perp = perpendicular(n);
    Coordinate global2Local(cross(perp, n), perp, n, x, 1);
    Coordinate dir(Vector3(1,0,0), Vector3(0,1,0), Vector3(0,0,1), omega, 0);

    Coordinate local2Global = inverseTransformation(global2Local);

    Vector3 globalDir = local2Global(dir).getPosition();

    return { globalDir, eval(x, globalDir, omega0) };
}