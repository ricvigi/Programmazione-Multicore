#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "utils.h"


double gsum = 0;
long long n;
long nthreads;
pthread_mutex_t mutex;

/* to approximate pi we use the formula pi = 4*(1/(2^n + 1)) */
void* approx_pi(void* rank) {
    long r = (long) rank;
    long long start = r * (n / nthreads);
    register long long finish = (r + 1) * (n/nthreads);
    register long n;
    double psum = 0.0;

    for (n = start; n <= finish; n++) {
        psum += pow(-1.0, n) * (1/((2.0 * n) + 1));
    }

    /* Critical section */
    pthread_mutex_lock(&mutex);
    gsum += psum;
    pthread_mutex_unlock(&mutex);
    return NULL;
}


/* pthread threads take into account logical cores, instead of
 * actual physical cores. To have maximum usage of CPU power, use
 * 8 threads */
int main(int argc, char** argv) {
    n = atoi(argv[1]); /* number of iterations */
    nthreads = atoi(argv[2]); /* number of threads */
    pthread_t* handles;
    handles = malloc(nthreads*sizeof(pthread_t));

    for (long rank = 0; rank < nthreads; rank++) {
        pthread_create(&handles[rank], NULL, approx_pi, (void*) rank);
    }

    for (int rank = 0; rank < nthreads; rank++) {
        pthread_join(handles[rank], NULL);
    }
    double pi = 4 * gsum;
    printf("pi=%f\n", pi);

    return 0;
}
