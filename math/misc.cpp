#include "misc.hpp"

float clampHigh(float val, float bound) {
    return val > bound ? bound : val;
}

float clampLow(float val, float bound) {
    return val < bound ? bound : val;
}