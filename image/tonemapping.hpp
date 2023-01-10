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

    Image simpleReinhard(const Image& image, const double a, const double delta);

    Image extendedReinhard(const Image& image, const double a, const double delta, const double lwhite);

    Image filmicUncharted(const Image& image);

    /* Local tone mapping operators */

    Image localReinhard(const Image& image,
        double key, double sharp, double center, double surround, double threshold);
}