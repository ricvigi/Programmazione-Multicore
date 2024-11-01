#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
#include "exercises.h"

int main(int argc, char **argv) {
    int* A;
    int* B;
    int* C;
    int m = 2;
    int n = 2;
    A = (int*) malloc(sizeof(int)*m*n);
    B = (int*) malloc(sizeof(int)*m*n);
    C = (int*) malloc(sizeof(int)*m*n);

    // populate the matrices
    int c = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            A[i * n + j] = c;
            B[i * n + j] = c;
            c++;
        }
    }

    free(A);
    free(B);
    free(C);
    return 0;
}
