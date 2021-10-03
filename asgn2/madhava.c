#include "mathlib.h"
#ifndef _NEWTON_
#include "newton.c"
#endif
static int terms2 = 0;
double pi_madhava(void){
	double sum = 0, numerator = 1, term=1;
	while(absolute(term) > EPSILON){
		term = (numerator / (2*terms2+1));
		sum += term;
		terms2+=1;
		numerator /= (-3);
	}
	return sqrt_newton(12) * sum;
}
int pi_madhava_terms(void){
	return terms2;
}

