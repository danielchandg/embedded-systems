#pragma once
#include "mathlib.h"
static int terms=0;
double pi_bbp(void){
	double sum = 0, sixteen_term = 1;
	while(1){
		double term = sixteen_term * (4/(8*terms+1) - 2/(8*terms+4) - 1/(8*terms+5) - 1/(8*terms+6));
		if(term <= EPSILON) break;
		sum += term;
		terms += 1;
		sixteen_term /= 16;
	}
	return sum;

}
int pi_bbp_terms(void){
	return terms;
}
