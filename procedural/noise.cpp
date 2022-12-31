#include "noise.hpp"

#include <memory>
#include <math/misc.hpp>
#include <math/interpolation.hpp>

SimpleNoise::SimpleNoise(unsigned int gs, double w, double h) 
    : gridSize(gs), width(w), height(h) {
    // Raw pointer allocation is freed on the destructor
    pointGrid = new double[gs * gs];
    RandomGenerator gen(0, 1);
    for ( int i = 0; i < gs * gs; i++ ) {
        pointGrid[i] = gen();
    }
}

SimpleNoise::~SimpleNoise() {
    // free allocated memory
    delete[] pointGrid;
}

// Inspired by https://www.scratchapixel.com/lessons/procedural-generation-virtual-worlds/procedural-patterns-noise-part-1/creating-simple-2D-noise
double SimpleNoise::operator()(const double u, const double v) const {

    // get the points of the square in which (u, v) lies
    int ix = positiveModulo(floor(u / width), gridSize);
    int iy = positiveModulo(floor(v / height), gridSize);

    // No need to do positive modulo since ix and iy are already positive
    int ix2 = (ix + 1) % gridSize;
    int iy2 = (iy + 1) % gridSize;

    // Compute the 2d smoothstep function instead of raw bilinear interpolation
    double sx = smoothstep(u / width - floor(u / width)); 
    double sy = smoothstep(v / width - floor(v / width));

    double nx0 = lerp(pointGrid[ix + iy * gridSize], pointGrid[ix2 + iy * gridSize], sx);
    double nx1 = lerp(pointGrid[ix + iy2 * gridSize], pointGrid[ix2 + iy2 * gridSize], sx);

    double l = lerp(nx0, nx1, sy);

    return l;
}

FractalNoise::FractalNoise(unsigned int gs, unsigned int oct, double w, double h) {
    for ( int i = 0; i < oct; i++ ) {
        octaves.push_back(make_shared<SimpleNoise>(gs, w / int(1 << i), h / int(1 << i)));
    }
}

double FractalNoise::operator()(const double u, const double v) const {

    double r = 0;
    for ( auto& o : octaves ) {
        r += (double)(*o)(u, v);
    } 

    return r / double(octaves.size());
}