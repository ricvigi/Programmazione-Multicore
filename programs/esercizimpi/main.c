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
    for (int i = 0; i < m; i++) {
        for (int j = 0; i < n; j++) {
            A[i * n + j] = i + 1;
            B[i * n + j] = i + 1;
        }
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("A[%d][%d] = %d\tB[%d][%d] = %d\n", i, j, A[i * n +j], i, j, B[i * n + j]);
        }
    }
    //
    // seq_matrix_matrix_mul(A, B, C, &m, &n);
    // for (int i = 0; i < 2; i++) {
    //     for (int j = 0; j < 2; j++) {
    //         printf("%d\t", C[i][j]);
    //     }
    //     puts("\n");
    // }

    free(A);
    free(B);
    free(C);
    return 0;
}
