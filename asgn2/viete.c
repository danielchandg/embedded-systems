#pragma once
#include "mathlib.h"
static int factors = 0;
double pi_viete(void){
	double product = 1, factor = 0;
	while(1){
		factor += 2;
		double a=0, b=factor, c;
		while(b-a > EPSILON){
			c=(b-a)/2;
			if(c*c > factor) b=c;
			else a=c;
		}
		factor = c;
		if(factor <= 2 * EPSILON) break;
		product *= factor/2;
		factors+=1;
	}
	return 2/product;
}
int pi_viete_factors(){
	return factors;
}
