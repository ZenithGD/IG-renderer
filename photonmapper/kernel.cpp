#include "kernel.hpp"

double boxKernel(double rp, double rk) {
    return 1.0 / ( M_PI * rk * rk );
}

double coneKernel(double rp, double rk) {

    return ( 1.0 - rp / ( M_PI * rk * rk ) ) / (2 * M_PI * rk - 1);
}

double epanechnikovKernel(double rp, double rk) {
    double dif = (rk - rp) / rk;

    return 3.0 * ( 1 - dif * dif ) / 4.0;
}
