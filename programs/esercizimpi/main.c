#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "exercises.h"

/* To access the correct index in a 3D matrix i*j*k stored as a
 * contiguous 1D array in memory, we use the formula:
 *                  Index = d*(i*j) + r*k + c,
 * where d is the depth index, r is the row index, and c is the
 * column index.*/
int main(int argc, char** argv) {
    int* A = (int*)malloc((3*2*2)*sizeof(int));
    for (int i = 1; i <= (3*2*2); i++){
        A[i] = i;
        printf("%d\n", A[i]);
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++){
                printf("A[%d][%d][%d] = %d at position: %d\n", i,
                       j, k, A[i*(2*2) + (j * 2) + k],
                       i*(2*2) + (j * 2) + k);
            }
        }
    }
    free(A);
    return 0;
}


