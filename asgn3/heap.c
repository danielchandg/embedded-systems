#include "heap.h"

#include "stats.h"

int32_t max_child(Stats *stats, uint32_t *A, int32_t first, int32_t last) {
    /*
	 * This function returns the max child of first.
	 * The children are at the indexes of 2*first and 2*first + 1.
	 */
    int32_t left = 2 * first, right = 2 * first + 1;
    return (right <= last && cmp(stats, A[right - 1], A[left - 1]) > 0) ? right : left;
}

void fix_heap(Stats *stats, uint32_t *A, int32_t first, int32_t last) {
    /*
	 * This function checks if mother is less than its max child.
	 * If not, then mother is swapped with max child, and the function
	 * repeats with the new mother.
	 * If it is, then the heap is OK for first.
	 */
    int32_t mother = first, great = max_child(stats, A, first, last);
    while (mother <= last / 2) {
        if (cmp(stats, A[mother - 1], A[great - 1]) < 0) {
            swap(stats, &A[mother - 1], &A[great - 1]);
            mother = great;
            great = max_child(stats, A, mother, last);
        } else
            break;
    }
}
void build_heap(Stats *stats, uint32_t *A, int32_t first, int32_t last) {
    /*
	 * This function calls fix_heap on every number in the first
	 * half of A. 
	 */
    for (int32_t i = last / 2; i > first - 1; i--) {
        fix_heap(stats, A, i, last);
    }
}
void heap_sort(Stats *stats, uint32_t *A, uint32_t n) {
    /*
	 * First runs build_heap, then calls fix_heap on every element
	 * of the array except the first element. 
	 */
    int32_t first = 1;
    build_heap(stats, A, first, n);
    for (int32_t i = n; i > first; i--) {
        swap(stats, &A[first - 1], &A[i - 1]);
        fix_heap(stats, A, first, i - 1);
    }
}
