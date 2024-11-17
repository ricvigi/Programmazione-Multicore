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
    int i = 1;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    if (s % comm_sz != 0) {
        printf("[*]ERROR, size of vector must be evenly divisible by number of processes\n");
        fflush(stdout);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    const int root = 0;
    const int local_sz = s / comm_sz;
    int recv[local_sz];

    if (rank == root) {
        int* A = create_random_vector(s);
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Allreduce(A, recv, local_sz,
                      MPI_INT, MPI_SUM, MPI_COMM_WORLD);
        print_mat(A, &i, &s);

    } else {
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Allreduce(NULL, recv, local_sz,
                      MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    }

    if (rank == root) {
        puts("\n");
        print_mat(recv, &i, &local_sz);
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
