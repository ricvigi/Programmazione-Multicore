#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <time.h>
#include "eserciziomp.h"
static long num_trials = 100000000;

void print_mat(int* A, int* m, int*n) {
    for (int i = 0; i < *m; i++) {
        for (int j = 0; j < *n; j++) {
            printf("%d ", A[i * *n + j]);
        }
        puts("\n");
    }
}

int* create_random_vector(int n, int max) {
    int* vec = (int*) malloc(n * sizeof(int));
    for(int i = 0; i < n; i++){
        unsigned int seed = omp_get_thread_num() + i;
        vec[i] = rand_r(&seed) % max;
    }
    return vec;
}


int Pi_mc() {
    long i;  long Ncirc = 0;
    double pi, x, y, test, total_time;
    double r = 1.0;   // radius of circle. Side of squrare is 2*r
    srand(time(NULL));

    total_time = omp_get_wtime();
    # pragma omp parallel
    for(i=0;i<num_trials; i++) {
        x = RANDOM_DOUBLE;
        y = RANDOM_DOUBLE;

        test = x*x + y*y;

        if (test <= r*r) Ncirc++;
    }

    pi = 4.0 * ((double)Ncirc/(double)num_trials);

    printf("\n %ld trials, pi is %f ",num_trials, pi);
    printf(" in %f seconds\n",omp_get_wtime()-total_time);

    return 0;
}
