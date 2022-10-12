#pragma once 

#include <iostream>

#include "image.hpp"

using namespace std;

namespace tonemapping {

    /* Global tone mapping operators */

    Image clamp(const Image& image);

    Image equalize(const Image& image);

    Image gamma(const Image& image, double gamma);

    Image equalizeClamp(const Image& image, double v);

    Image gammaClamp(const Image& image, double v, double gamma);

    Image simpleReinhard(const Image& image);

    Image extendedReinhard(const Image& image, double maxWhite);

    /* Local tone mapping operators */

    Image gaussianKernel(const Image& image, unsigned int kernelSize, double sigma);
}