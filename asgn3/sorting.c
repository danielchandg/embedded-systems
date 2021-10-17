#include "heap.h"
#include "insert.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stats.h"

#include <errno.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "aeisqr:n:p:h"

// Prints out the help string given the current directory.
void help_string(char *cwd) {
    printf(
        "SYNOPSIS\n   A collection of comparison-based sorting algorithms.\n\nUSAGE\n   %s/sorting "
        "[-haeisqn:p:r:] [-n length] [-p elements] [-r seed]\n\nOPTIONS\n   -h              "
        "display program help and usage.\n   -a              enable all sorts.\n   -e              "
        "enable Heap Sort.\n   -i              enable Insertion Sort.\n   -s              enable "
        "Shell Sort.\n   -q              enable Quick Sort.\n   -n length       specify number of "
        "array elements (default: 100).\n   -p elements     specify number of elements to print "
        "(default: 100).\n   -r seed         specify random seed (default: 13371453).\n",
        cwd);
}

// Prints out <elements> amount of numbers from A with 5 numbers per row.
void print_arr(uint32_t *A, uint32_t elements) {
    for (uint32_t i = 0; i < elements; i++) {
        if (i % 5 == 0)
            printf("\n");
        printf("%13" PRIu32, A[i]);
    }
    printf("\n");
}

int compare(const void *a, const void *b) {
    return (*(int *) b - *(int *) a);
}

int main(int argc, char **argv) {
    int64_t opt = 0;
    uint32_t seed = 13371453, elements = 100, print = 100;
    Set opts = empty_set();
    char *cwd = (char *) calloc(FILENAME_MAX, sizeof(char));
    getcwd(cwd, FILENAME_MAX);

    Stats heap_stats, insert_stats, shell_stats, quick_stats;
    // Stats *stats = (Stats *)calloc(4, sizeof(Stats));
    // char *sort_name[4] = {"Heap", "Shell", "Insertion", "Quick"};
    // void (*sorts[])(*Stats, *uint32_t, uint32_t) = {heap_sort, shell_sort, insertion_sort, quick_sort};

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a': opts |= 15; break;
        case 'e': opts = insert_set(0, opts); break;
        case 's': opts = insert_set(1, opts); break;
        case 'i': opts = insert_set(2, opts); break;
        case 'q': opts = insert_set(3, opts); break;
        case 'r':
            seed = strtoul(optarg, NULL, 10);
            // Checks for invalid seed
            if (errno == EINVAL) {
                // Sets seed to 1 if passed argument isn't a number
                seed = 1;
            }
            break;
        case 'n':
            elements = strtoul(optarg, NULL, 10);
            if (errno == EINVAL) {
                elements = 0;
            }
            break;
        case 'p':
            print = strtoul(optarg, NULL, 10);
            if (errno == EINVAL) {
                print = 0;
            }
            break;
        case 'h': help_string(cwd); return 0;
        case '?': help_string(cwd); return 0;
        }
    }
    // check if no sort options are selected
    if (opts == empty_set()) {
        fprintf(stderr, "Select at least one sort to perform.\n");
        help_string(cwd);
        return 0;
    }
    free(cwd);

    // If print is more than the size of the array, set print to array size
    if (print > elements) {
        print = elements;
    }

    // arr is the unsorted array
    // A is the sorted array
    uint32_t *arr = (uint32_t *) calloc(elements, sizeof(uint32_t));
    uint32_t *A = (uint32_t *) calloc(elements, sizeof(uint32_t));
    srandom(seed);

    for (uint32_t i = 0; i < elements; i++) {
        // 1073741824 is 2^30
        arr[i] = random() % 1073741824;
        A[i] = arr[i];
    }

    //qsort(arr, elements, sizeof(uint32_t), compare);
    for (uint32_t i = 0; i < elements; i++)
        A[i] = arr[i];

    if (member_set(0, opts)) {
        reset(&heap_stats);
        heap_sort(&heap_stats, A, elements);
        printf("Heap Sort, %" PRIu32 " elements, %" PRIu64 " moves, %" PRIu64 " compares", elements,
            heap_stats.moves, heap_stats.compares);
        print_arr(A, print);
        for (uint32_t i = 0; i < elements; i++)
            A[i] = arr[i];
    }
    if (member_set(1, opts)) {
        reset(&shell_stats);
        shell_sort(&shell_stats, A, elements);
        printf("Shell Sort, %" PRIu32 " elements, %" PRIu64 " moves, %" PRIu64 " compares",
            elements, shell_stats.moves, shell_stats.compares);
        print_arr(A, print);
        for (uint32_t i = 0; i < elements; i++)
            A[i] = arr[i];
    }

    if (member_set(2, opts)) {
        reset(&insert_stats);
        insertion_sort(&insert_stats, A, elements);
        printf("Insertion Sort, %" PRIu32 " elements, %" PRIu64 " moves, %" PRIu64 " compares",
            elements, insert_stats.moves, insert_stats.compares);
        print_arr(A, print);
        for (uint32_t i = 0; i < elements; i++)
            A[i] = arr[i];
    }
    if (member_set(3, opts)) {
        reset(&quick_stats);
        quick_sort(&quick_stats, A, elements);
        printf("Quick Sort, %" PRIu32 " elements, %" PRIu64 " moves, %" PRIu64 " compares",
            elements, quick_stats.moves, quick_stats.compares);
        print_arr(A, print);
        for (uint32_t i = 0; i < elements; i++)
            A[i] = arr[i];
    }

    free(arr);
    free(A);
    return 0;
}
