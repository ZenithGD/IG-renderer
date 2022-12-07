#pragma once

#include <random>

double clampHigh(double val, double bound);

double clampLow(double val, double bound);

class RandomGenerator {
    public:
        RandomGenerator(double lo, double hi);

        double operator()();
    private:

        std::uniform_real_distribution<double> distribution;
        std::default_random_engine generator;
};