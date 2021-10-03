#pragma once
#include "mathlib.h"
static int terms1 = 1;
double e(void){
	double sum = 1, term = 1;
	while(term > EPSILON){
		term /= terms1;
		sum+=term;
		terms1+=1;
	}
	return sum;
}
int e_terms(void){
	return terms1;
}
