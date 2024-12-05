#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "utils.h"

#define ITERATIONS 1000008

long nthreads, a, b;
double h, approx;
pthread_mutex_t mutex;

/* Compute the area under the curve of the square x^2 function */
void* trapezoidal_rule(void* rank) {
    long r = (long) rank;
    long local_n = ITERATIONS / nthreads;
    double local_a = a + (r*local_n*h);
    double local_approx = 0.0;

    for (int i = 1; i <= local_n; i++) {
        double x_i = local_a + i*h;
        local_approx += square(x_i);
    }

    pthread_mutex_lock(&mutex);
    // printf("Thread %ld, local_approx=%lf\n", r, local_approx);
    // fflush(stdout);
    approx += local_approx;
    pthread_mutex_unlock(&mutex);

    return NULL;
}


int main(int argc, char** argv) {
    nthreads = atoi(argv[1]);
    a = atoi(argv[2]);
    b = atoi(argv[3]);
    pthread_t* handles;
    handles = malloc(nthreads*sizeof(pthread_t));

    if (ITERATIONS % nthreads != 0) {
        printf("[*] Error, ITERATIONS (%d) must be evenly divisible by number of threads\n", ITERATIONS);
        exit(EXIT_FAILURE);
    }
    h = (double)(b - a) / ITERATIONS;
    approx = (square(a) + square(b)) / 2.0;

    for (long rank = 0; rank < nthreads; rank++) {
        pthread_create(&handles[rank], NULL, trapezoidal_rule, (void*) rank);
    }

    for (long rank = 0; rank < nthreads; rank++) {
        pthread_join(handles[rank], NULL);
    }
    approx = h*approx;
    printf("Area under x^2 between %ld and %ld is: %lf", a, b, approx);
    free(handles);
    return 0;
}
