#include "misc.hpp"

double clampHigh(double val, double bound) {
    return val > bound ? bound : val;
}

double clampLow(double val, double bound) {
    return val < bound ? bound : val;
}