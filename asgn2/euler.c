#pragma once
#include "mathlib.h"
#include "newton.c"
static int terms3 = 1;
double pi_euler(void){
	double sum = 0, term=1;
	while(term > EPSILON){
		double t = (double)terms3;
		term = 1/t/t;
		sum += term;
		terms3+=1;
	}
	terms3 -= 1;
	return sqrt_newton(6*sum);
}
int pi_euler_terms(void){
	return terms3;
}
