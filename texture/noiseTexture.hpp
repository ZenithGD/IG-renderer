#pragma once

#include <texture/texture.hpp>
#include <procedural/noise.hpp>
#include <math/interpolation.hpp>

#include <memory>

class NoiseTexture : public Texture<RGB> {
public:

    NoiseTexture(const shared_ptr<Noise>& n, const RGB& _ca, const RGB& _cb)
        : noise(n), ca(_ca), cb(_cb) {}

    RGB sample(const double u, const double v, const Vector3& p) const override {
        return colorLerp(ca, cb, (*noise)(u, v));
    }
private:
    shared_ptr<Noise> noise;
    RGB ca, cb;
};
