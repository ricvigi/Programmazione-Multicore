#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "utils.h"


long m;
long n;
long nthreads;
long* A;
long* y;
long* res;

/* Performs A @ y.T */
void* mat_vec_mul(void* rank) {
    long r = (long) rank;
    long start = r * (m / nthreads);
    long finish = (r + 1) * (m / nthreads) - 1;
    for (long i = start; i <= finish; i++) {
        res[i] = 0;
        for (long j = 0; j < n; j++) {

            res[i] += A[i * n + j] * y[j];
        }
    }
    return NULL;
}


int main(int argc, char** argv) {
    m = atoi(argv[1]);
    n = atoi(argv[2]);
    nthreads = atoi(argv[3]);
    A = create_random_vector(m * n);
    y = create_random_vector(n);
    res = create_zero_vector(m);

    pthread_t* handles;

    handles = malloc(nthreads*sizeof(pthread_t));

    for (long rank = 0; rank < nthreads; rank++) {
        pthread_create(&handles[rank], NULL, mat_vec_mul, (void*) rank);
    }

    for (long i = 0; i < nthreads; i++) {
        pthread_join(handles[i], NULL);
    }

    long i = 1;
    prinf("A:\n");
    print_mat(A, &m, &n);
    printf("y:\n");
    print_mat(y, &i, &n);
    printf("res:\n");
    print_mat(res, &i, &m);
    free(A);
    free(y);
    free(res);
    return 0;
}
