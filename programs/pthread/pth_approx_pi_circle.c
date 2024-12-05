#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "utils.h"


long nthreads;
long ncircle;
long ntosses;
long local_iter;
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
    ntosses = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    ncircle = 0;
    pthread_t* handles;
    handles = malloc(nthreads*sizeof(pthread_t));

    if (ntosses % nthreads != 0) {
        printf("[*] Error, number of iterations should be evenly divisible by number of threads\n");
        exit(EXIT_FAILURE);
    }

    local_iter = ntosses / nthreads;

    for (long rank = 0; rank < nthreads; rank++) {
        pthread_create(&handles[rank], NULL, approx_pi, (void*) rank);
    }
    for (long rank = 0; rank < nthreads; rank++) {
        pthread_join(handles[rank], NULL);
    }
    double pi = 4.0*((double)ncircle/ntosses);
    printf("pi=%lf\n", pi);

    free(handles);
    return 0;
}
