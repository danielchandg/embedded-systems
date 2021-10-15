#include "stats.h"
#include "insert.h"
#include "shell.h"
#include "heap.h"
#include "quick.h"
#include "set.h"

#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include <errno.h>
#include <stdbool.h>

#define OPTIONS "aeisqr:n:p:h"

void help_string(char *cwd){
	printf(("SYNOPSIS\n   A collection of comparison-based sorting algorithms.\n\nUSAGE\n   %s/sorting [-haeisqn:p:r:] [-n length] [-p elements] [-r seed]\n\nOPTIONS\n   -h              display program help and usage.\n   -a              enable all sorts.\n   -e              enable Heap Sort.\n   -i              enable Insertion Sort.\n   -s              enable Shell Sort.\n   -q              enable Quick Sort.\n   -n length       specify number of array elements (default: 100).\n   -p elements     specify number of elements to print (default: 100).\n   -r seed         specify random seed (default: 13371453).\n", cwd);
}

int main(int argc, char **argv){
	int32_t opt = 0;
	uint32_t seed = 0, elements = 100, print = 100;
	Set opts = empty_set();
	Stats heap_stats;
	reset(&heap_stats);


	int32_t *B = (uint32_t *)calloc(elements, sizeof(uint32_t));
	free(B);

	while((opt = getopt(argc, argv, OPTIONS)) != -1){
		switch (opt) {
		case 'a':
			insert_set(15, opts);
			break;
		case 'e':
			insert_set(1, opts);
			break;
		case 'i':
			insert_set(2, opts);
			break;
		case 's':
			insert_set(4, opts);
			break;
		case 'q':
			insert_set(8, opts);
			break;
		case 'r':
			printf("argument is: %s\n", optarg);
			seed = strtoul(optarg, NULL, 10);
			// Checks for invalid seed
			if(errno == EINVAL){
				
			}
			break;
			

	printf("seed = %" PRIu32 "\n", seed);
	printf("Bubble Sort, %" PRIu32 " elements, ", elements);
