#include "mathlib.h"
static int iters = 0;
double sqrt_newton(double x){
	double z=0, y=1;
	while(absolute(y-z) > EPSILON){
		z = y;
		y = 0.5 * (z + x / z);
		iters += 1;
	}
	return y;
}
int sqrt_newton_iters(void){
	return iters;
}

