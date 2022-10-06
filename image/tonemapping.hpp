#pragma once 

#include <iostream>

#include "image.hpp"

using namespace std;

namespace tonemapping {

    Image clamp(const Image& image);

    Image equalize(const Image& image);

    Image gamma(const Image& image, float gamma);

    Image equalizeClamp(const Image& image, float v);

    Image gammaClamp(const Image& image, float v, float gamma);

}