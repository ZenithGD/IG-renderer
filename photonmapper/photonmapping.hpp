#pragma once

#include <image/image.hpp>
#include <scene/scene.hpp>

using namespace std;

struct PhotonConfig {
    unsigned int total, maxPhotons, bounces;
    double photonBoost, causticBoost;
    bool nee;
};

Image photonMapping(const Scene& sc, const PhotonConfig& pc);
