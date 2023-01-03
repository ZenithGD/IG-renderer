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

    Image gammaClamp(const Image& image, double gamma);

    Image simpleReinhard(const Image& image);

    Image extendedReinhard(const Image& image, double maxWhite);

    Image filmicUncharted(const Image& image);

    /* Local tone mapping operators */

    Image localReinhard(const Image& image,
        double key, double sharp, double center, double surround, double threshold);
}