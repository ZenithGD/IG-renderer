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

    double operator()(const double u, const double v) const override;

private:
    vector<shared_ptr<SimpleNoise>> octaves;
};

class TurbulentNoise : public Noise {
public:

    TurbulentNoise(const shared_ptr<Noise>& n)
        : noise(n) {}
    
    double operator()(const double u, const double v) const override {

        return abs((*noise)(u, v) * 2 - 1);
    }
private:
    shared_ptr<Noise> noise;
};

class WoodyNoise : public Noise {
public:

    WoodyNoise(const shared_ptr<Noise>& n, double amp)
        : noise(n), amplitude(amp) {}
    
    double operator()(const double u, const double v) const override {

        double val = (*noise)(u, v) * amplitude;
        return val - (int)val;
    }
private:
    shared_ptr<Noise> noise;
    double amplitude;
};