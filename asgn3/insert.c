#include "insert.h"
#include "stats.h"
void insertion_sort(Stats *stats, uint32_t *A, uint32_t n){
	for(uint32_t i=1; i<n; i++){
		uint32_t j = i, temp = move(stats, A[i]);
		// Move A[j] to the left until it reaches an element
		// greater than or equal to it, or reaches the end.
		while(j > 0 && cmp(stats, temp, A[j-1]) < 0){
			// This is the "move" in insertion sort
			A[j] = move(stats, A[j-1]);
			j--;
		}
		A[j] = move(stats, temp);
	}
}
