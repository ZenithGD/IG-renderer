#pragma once

#include <image/image.hpp>
#include <scene/scene.hpp>

using namespace std;

Image photonMapping(const Scene& sc, const unsigned int total, const unsigned int maxPhotons);

Image getPhotonMap(const Scene& sc, const unsigned int total, const unsigned int maxPhotons);