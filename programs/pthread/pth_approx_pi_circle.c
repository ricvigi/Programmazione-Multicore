#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "utils.h"


long nthreads;
long ncircle;
long ntosses;
long local_iter;
const pthread_mutexattr_t attr_p;
pthread_mutex_t mutex;

void* approx_pi(void* rank) {
    long local_ncircle = 0;

    for (int i = 0; i < local_iter; i++) {
        double x = RANDOM_DOUBLE;
        double y = RANDOM_DOUBLE;
        double z = x*x + y*y;
        if (z <= 1) local_ncircle++;
    }

    pthread_mutex_lock(&mutex);
    ncircle += local_ncircle;
    pthread_mutex_unlock(&mutex);


    return NULL;
}


int main(int argc, char** argv) {
    long* ranks;
    pthread_t* handles;
    ntosses = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    ncircle = 0;
    pthread_mutex_init(&mutex, &attr_p);
    ranks = (long*)malloc(nthreads*sizeof(long));
    handles = (pthread_t*)malloc(nthreads*sizeof(pthread_t));

    if (ntosses % nthreads != 0) {
        printf("[*] Error, number of iterations should be evenly divisible by number of threads\n");
        exit(EXIT_FAILURE);
    }

    local_iter = ntosses / nthreads;

    /* ATTENTION: you can't use the variable rank inside
     * pthread_create. Since the function has an immediate return,
     * you have no guarantee the thread will be created before
     * the variable rank is updated! */
    for (long rank = 0; rank < nthreads; rank++) {
        ranks[rank] = rank;
        pthread_create(&handles[rank], NULL, approx_pi, (void*) ranks[rank]);
    }
    for (long rank = 0; rank < nthreads; rank++) {
        pthread_join(handles[rank], NULL);
    }
    double pi = 4.0*((double)ncircle/ntosses);
    printf("pi=%lf\n", pi);

    pthread_mutex_destroy(&mutex);
    free(handles);
    free(ranks);
    return 0;
}
