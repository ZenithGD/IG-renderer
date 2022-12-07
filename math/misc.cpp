#include "misc.hpp"

#include <functional>

double clampHigh(double val, double bound) {
    return val > bound ? bound : val;
}

double clampLow(double val, double bound) {
    return val < bound ? bound : val;
}

RandomGenerator::RandomGenerator(double lo, double hi) 
    : distribution(lo, hi) {

    std::random_device rand_dev;
    generator = std::default_random_engine(rand_dev());
}

double RandomGenerator::operator()() {

    return distribution(generator);
}
