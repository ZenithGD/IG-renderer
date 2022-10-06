#pragma once 

#include <iostream>

#include "image.hpp"

using namespace std;

Image clamping(const Image& image);

Image equalization(const Image& image);

Image gamma(const Image& image, float k, float gamma);

Image equalizeClamp(const Image& image, float v);

Image gammaClamp(const Image& image, float v);