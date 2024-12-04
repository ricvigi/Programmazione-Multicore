#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>



void* foo(void* a) {
    long* px = (long*) a;
    long x = *px;
    printf("x=%ld\n", x);
    fflush(stdout);
    return NULL;
}

void print_mat(long* A, long* m, long* n) {
    for (long i = 0; i < *m; i++) {
        for (long j = 0; j < *n; j++) {
            printf("%ld ", A[i * *n + j]);
        }
        puts("\n");
    }
}

/* Creates a random vector of integers of size n. The integers are in the
 * range [0,9]. Change the modulo in vec[i] = rand() % k to change the range */
long* create_random_vector(long n) {
    long* vec = (long*) malloc(n * sizeof(long));
    for(long i = 0; i < n; i++){
        vec[i] = rand() % 10;
    }
    return vec;
}

/* Creates a zeroed vector of integers of size n */
long* create_zero_vector(long n) {
    long* vec = (long*) malloc(n*sizeof(long));
    for (long i = 0; i < n; i++) {
        vec[i] = 0;
    }
    return vec;
}
