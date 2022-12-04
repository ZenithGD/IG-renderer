#include <material/Emitter.hpp>

RGB Emitter::eval(const Vector3& x, const Vector3& omegaI, const Vector3& omega, const Intersection& it) const {
    return emission;
}

optional<tuple<Vector3, RGB>> Emitter::sample(const Vector3& omega0, const Vector3& x, const Intersection& it) const {
    return make_optional<tuple<Vector3, RGB>>(Vector3(), emission);
}
