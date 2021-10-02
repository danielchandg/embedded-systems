#pragma once
#include "mathlib.h"
#include "newton.c"
static int terms = 0;
double pi_madhava(void){
	double sum = 0, numerator = 1;
	while(1){
		double term = (numerator / (2*terms+1));
		if(term <= EPSILON) break;
		sum += term;
		terms+=1;
		numerator /= (-3);
	}
	return sqrt_newton(12) * sum;
}
int pi_madhava_terms(void){
	return terms;
}

