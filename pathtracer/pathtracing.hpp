#pragma once

#include <image/image.hpp>
#include <scene/scene.hpp>

using namespace std;

/**
 * @brief Render the scene onto an image with the path tracing algorithm.
 * 
 * @param sc The scene
 * @return Image The rendered image
 */
Image pathTracing(const Scene& sc);

