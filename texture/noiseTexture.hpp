#pragma once

#include <texture/texture.hpp>

#include <memory>

class NoiseTexture : public Texture<RGB> {
public:

    NoiseTexture();

    RGB sample(const double u, const double v, const Vector3& p) const override;
private:
    Noise noise;
};

class Noise {
    
};