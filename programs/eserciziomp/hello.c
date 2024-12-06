#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>
#include "eserciziomp.h"

void Hello(void);

int main (int argc, char** argv) {
    /* Alternative, you can export OMP_NUM_THREADS=n to have
     * pragma omp parallel spawn n threads. */
    // int thread_count = atoi(argv[1]);

#   pragma omp parallel
    Hello();
    return 0;
}


void Hello(void) {
    int rank = omp_get_thread_num();
    int nthreads = omp_get_num_threads();

    printf("Hello from thread %d of %d\n", rank, nthreads);
}


