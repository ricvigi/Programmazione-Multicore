#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
#include "exercises.h"

int main(int argc, char** argv) {
    /* Matrix sizes */
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    int p = (m * n);

    int rank, comm_sz;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);


    /* Check for whole division between matrix size and number
     * of processes. */
    if (p % comm_sz != 0) {
        printf("[*]ERROR, array size should be divisible by number of processes\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    if (m != n) {
        printf("[*]ERROR, we are only dealing with square matrices at the moment\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    const int local_sz = p / comm_sz;
    const int root = comm_sz - 1;


    /* destination of SCATTERED matrix */
    int dest[local_sz];

    /* destination of BROADCASTED matrix */
    int dest1[p];

    /* rank p - 1 should generate the matrices and
     * scatter them */
    if (rank == root) {
    int* A = create_random_vector(p);
    int* B = create_random_vector(p);
    int C[p];

    /* Scatter A matrix */
    MPI_Scatter(A, local_sz,
                MPI_INT, dest,
                local_sz, MPI_INT,
                root, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    /* Broadcast B matrix */
    MPI_Bcast (B, p, MPI_INT, root, MPI_COMM_WORLD);
    int res[local_sz];
    mat_vect_mult(&dest1[0], &dest[0], &res[0], &m, &n);

    free(A);
    free(B);
    }
     else {
        /* Receive scattered A matrix into dest */
        MPI_Scatter(NULL, local_sz,
                    MPI_INT, dest,
                    local_sz, MPI_INT,
                    root, MPI_COMM_WORLD);

        MPI_Barrier(MPI_COMM_WORLD);
        /* Receive broadcasted B matrix into dest1 */
        MPI_Bcast (dest1, p, MPI_INT, root, MPI_COMM_WORLD);
        int res[local_sz];
        mat_vect_mult(&dest1[0], &dest[0], &res[0], &m, &n);

    }
        if (rank == 0) {
        /* result matrix */
        int C[p];
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
