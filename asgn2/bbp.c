#pragma once
#include "mathlib.h"
static int terms4=0;
double pi_bbp(void){
	double sum = 0, sixteen_term = 1, term=1;
	while(term > EPSILON){
		double t = (double)terms4;
		term = sixteen_term * (4/(8*t+1) - 2/(8*t+4) - 1/(8*t+5) - 1/(8*t+6));
		sum += term;
		terms4 += 1;
		sixteen_term /= 16;
	}
	return sum;

}
int pi_bbp_terms(void){
	return terms4;
}
