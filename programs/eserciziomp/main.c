#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>
#include "eserciziomp.h"

#define ARRAY_SIZE 10000000

/* Implements counting sort. First argument from command line is the
 * maximum value that each element in the array can have. e.g., if the
 * argument is 100, then each element of the array can contain a value
 * between 0 and 100. You can assume that the size of the array is much
 * larger than the maximum value. */

int main (int argc, char** argv) {
    int max = atoi(argv[1]);
    int l = 1;
    int* A = create_random_vector(ARRAY_SIZE, max);
    int* counts = (int*)malloc(max*sizeof(int));

    return 0;
}



