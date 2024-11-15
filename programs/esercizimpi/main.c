#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
#include "exercises.h"

int main(int argc, char** argv) {
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    parallel_mat_mat_mul(&m, &n);
    return 0;
}
