#pragma once 

#include <vector>
#include <memory>

using namespace std;

/**
 * @brief Interface for procedural noise generators
 */
class Noise {
public:
    virtual double operator()(const double u, const double v) const = 0;
};

/**
 * @brief Class for representing simple 2d perlin noise
 * 
 */
class SimpleNoise : public Noise {
public:

    SimpleNoise(unsigned int gs, double w, double h);

    ~SimpleNoise();

    double operator()(const double u, const double v) const override;

private:
    double* pointGrid;
    unsigned int* permutationTable;
    unsigned int gridSize;

    double width, height;
};

class FractalNoise : public Noise {
public:

    FractalNoise(unsigned int gs, unsigned int oct, double w, double h);

    double operator()(const double u, const double) const override;

private:
    vector<shared_ptr<SimpleNoise>> octaves;
};
