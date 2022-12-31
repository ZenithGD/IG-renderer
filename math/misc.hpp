#pragma once

#include <random>

/**
 * @brief Clamp val by a higher bound
 * 
 * This is equivalent to min(bound, val)
 * @param val The value
 * @param bound The high bound
 * @return double 
 */
double clampHigh(double val, double bound);

/**
 * @brief Clamp val by a lower bound
 * 
 * This is equivalent to max(bound, val)
 * @param val The value
 * @param bound The lower bound
 * @return double 
 */
double clampLow(double val, double bound);

/**
 * @brief Clamp val to an interval [lo, hi]
 * 
 * This is equivalent to min(hi, max(lo, val))
 * @param val The value
 * @param bound The lower bound
 * @return double 
 */
double clampInterval(double val, double lo, double hi);

inline int positiveModulo(int a, int n) {
    return (a % n + n) % n;
}

class RandomGenerator {
    public:
        RandomGenerator(double lo, double hi);

        double operator()();
    private:

        std::uniform_real_distribution<double> distribution;
        std::default_random_engine generator;
};