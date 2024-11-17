#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
#include "exercises.h"


/* Exercise 2: Relying only on point-to-point MPI calls, implement a function
 * with the same signature/arguments and behaviour of MPI_Allreduce */
int main(int argc, char** argv) {
    /* give size of random vector as a command line argument */
    int s = atoi(argv[1]);
    int rank, comm_sz;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    if (s % comm_sz != 0) {
        printf("[*]ERROR, size of vector must be evenly divisible by number of processes\n");
        fflush(stdout);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    /* root process */
    int root = 0;

    if (rank == root) {
        int* A = create_random_vector(s);
        int res = 0;
        seq_vec_sum(A, &res, &s);
        printf("%d\n", res);
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
