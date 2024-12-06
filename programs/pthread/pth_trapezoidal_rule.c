#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "utils.h"

#define ITERATIONS 1000008

long nthreads, a, b;
double h, approx;
const pthread_mutexattr_t attr_p;
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
    long* ranks;
    pthread_t* handles;
    nthreads = atoi(argv[1]);
    a = atoi(argv[2]);
    b = atoi(argv[3]);
    handles = (pthread_t*)malloc(nthreads*sizeof(pthread_t));
    ranks = (long*)malloc(nthreads*sizeof(long));
    pthread_mutex_init(&mutex, &attr_p);

    if (ITERATIONS % nthreads != 0) {
        printf("[*] Error, ITERATIONS (%d) must be evenly divisible by number of threads\n", ITERATIONS);
        exit(EXIT_FAILURE);
    }
    h = (double)(b - a) / ITERATIONS;
    approx = (square(a) + square(b)) / 2.0;

    /* ATTENTION: you can't use the variable rank inside
     * pthread_crate. Since the function has an immediate return,
     * you have no guarantee the thread will be created before
     * the variable rank is updated! */
    for (long rank = 0; rank < nthreads; rank++) {
        ranks[rank] = rank;
        pthread_create(&handles[rank], NULL, trapezoidal_rule, (void*) ranks[rank]);
    }

    for (long rank = 0; rank < nthreads; rank++) {
        pthread_join(handles[rank], NULL);
    }
    approx = h*approx;
    printf("Area under x^2 between %ld and %ld is: %lf", a, b, approx);
    pthread_mutex_destroy(&mutex);
    free(handles);
    free(ranks);
    return 0;
}
