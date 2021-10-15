#include <math.h>
#include "stats.h"
#include "shell.h"
void shell_sort(Stats *stats, uint32_t *A, int32_t n){
	int32_t largest_gap = int32_t(log(2*n + 3) / log(3));
	for(int32_t k=largest_gap; k>0; k--){
		int32_t gap = (pow(3, k) - 1) / 2;
		for(int32_t i=gap; i<n; i++){
			int32_t j = i, temp = A[i];
			while(j >= gap && cmp(stats, temp, A[j-gap]) < 0){
				A[j] = move(stats, A[j-gap]);
				j -= gap;
			}
			A[j] = temp;
		}
	}
}
