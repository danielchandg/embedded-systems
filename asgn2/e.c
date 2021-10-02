#pragma once
#include "mathlib.h"
static int terms = 1;
double e(void){
	double sum = 1, term = 1;
	while(1){
		term /= terms;
		if(term <= EPSILON) break;
		sum+=term;
		terms+=1;
	}
	return sum;
}
int e_terms(void){
	return terms;
}
