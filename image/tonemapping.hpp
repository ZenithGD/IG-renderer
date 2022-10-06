#pragma once 

#include <iostream>

#include "image.hpp"

using namespace std;

namespace tonemapping {

    Image clamp(const Image& image);

    Image equalize(const Image& image);

    Image gamma(const Image& image, double gamma);

    Image equalizeClamp(const Image& image, double v);

    Image gammaClamp(const Image& image, double v, double gamma);

}