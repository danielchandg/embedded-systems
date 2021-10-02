#pragma once
#include "mathlib.h"
#include "newton.c"
static int terms = 1;
double pi_euler(void){
	double sum = 0;
	while(1){
		double term = 1/(terms*terms);
		if(term <= EPSILON) break;
		sum += term;
		terms+=1;
	}
	terms -= 1;
	return newton_sqrt(6*sum);
}
int pi_euler_terms(void){
	return terms;
}
