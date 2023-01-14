#include <material/Emitter.hpp>

RGB Emitter::eval(const Vector3& x, const Vector3& omegaI, const Vector3& omega, const Intersection& it) const {
    return emission;
}

optional<BRDFInteraction> Emitter::sample(const Vector3& omega0, const Vector3& x, const Intersection& it) const {
    return make_optional<BRDFInteraction>(Vector3(), emission, false);
}

RGB TextureEmitter::eval(const Vector3& x, const Vector3& omegaI, const Vector3& omega, const Intersection& it) const {
    return emission->sample(it.u, it.v, x);
}

optional<BRDFInteraction> TextureEmitter::sample(const Vector3& omega0, const Vector3& x, const Intersection& it) const {
    return make_optional<BRDFInteraction>(Vector3(), emission->sample(it.u, it.v, x), false);
}