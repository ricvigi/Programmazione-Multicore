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
    int i = 1; // This is needed to print vectors...

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    if (s % comm_sz != 0) {
        printf("[*]ERROR, size of vector must be evenly divisible by number of processes\n");
        fflush(stdout);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    int root = 0;
    int local_sz = s / comm_sz;
    int scatter_recv[local_sz];
    int res[local_sz];

    if (rank == root) {
        int* A = create_random_vector(s);
        print_mat(A, &i, &s);
        MPI_Barrier(MPI_COMM_WORLD);
        /* We scatter the generated vector before applying
         * Allreduce */
        MPI_Scatter(A, local_sz,
                    MPI_INT, scatter_recv,
                    local_sz, MPI_INT,
                    root, MPI_COMM_WORLD);
        free(A);
    } else {
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Scatter(NULL, local_sz,
                    MPI_INT, scatter_recv,
                    local_sz, MPI_INT,
                    root, MPI_COMM_WORLD);
    }
    MPI_Allreduce_custom_optimized(
                    &scatter_recv,
                    &res,
                    local_sz,
                    MPI_INT,
                    MPI_SUM,
                    MPI_COMM_WORLD);
    printf("process %d: ", rank);
    print_mat(&res[0], &i, &local_sz);
    printf("\n");

    MPI_Finalize();
    return EXIT_SUCCESS;
}
