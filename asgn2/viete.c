#include "mathlib.h"
static int factors = 0;
double pi_viete(void) {
    double product = 1, factor = 0;
    while (1) {
        factor = sqrt_newton(factor + 2);
        if (absolute(factor - 2) <= EPSILON)
            break;
        product *= factor / 2;
        factors += 1;
    }
    return 2 / product;
}
int pi_viete_factors() {
    return factors;
}
