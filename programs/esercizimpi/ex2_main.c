#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

void print_mat(int* A, int* m, int*n);
int* create_random_vector(int n);
int MPI_Allreduce_custom_optimized(
                void*       input_data_p    /* in  */,
                void*       output_data_p   /* out */,
                int         count           /* in  */,
                MPI_Datatype datatype       /* in  */,
                MPI_Op      operator        /* in  */,
                MPI_Comm    MPI_Comm        /* in  */);


/* Exercise 2: Relying only on point-to-point MPI calls,
 * implement a function with the same signature/arguments and
 * behaviour of MPI_Allreduce */

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
        MPI_Scatter(A, local_sz,
                    MPI_INT, scatter_recv,
                    local_sz, MPI_INT,
                    root, MPI_COMM_WORLD);
        free(A);
    } else {
        MPI_Barrier(MPI_COMM_WORLD);
        /* We scatter the generated vector before applying
         * Allreduce */
        MPI_Scatter(NULL, local_sz,
                    MPI_INT, scatter_recv,
                    local_sz, MPI_INT,
                    root, MPI_COMM_WORLD);
    }
    MPI_Allreduce_custom_optimized(
                    scatter_recv,
                    res,
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


/* Print a matrix A of size m * n */
void print_mat(int* A, int* m, int*n) {
    for (int i = 0; i < *m; i++) {
        for (int j = 0; j < *n; j++) {
            printf("%d ", A[i * *n + j]);
        }
        puts("\n");
    }
}

/* Creates a random vector of integers of size n. Integers are in
 * the range [0,9]. Change the modulo in vec[i] = rand() % k to
 * change the range */
int* create_random_vector(int n) {
    int* vec = (int*) malloc(n * sizeof(int));
    for(int i = 0; i < n; i++){
        vec[i] = rand() % 10;
    }
    return vec;
}

/* Allreduce with O(2log_{2}(p)) of sends and receives for
 * each process */
int MPI_Allreduce_custom_optimized(
                void*       input_data_p    /* in  */,
                void*       output_data_p   /* out */,
                int         count           /* in  */,
                MPI_Datatype datatype       /* in  */,
                MPI_Op      operator        /* in  */,
                MPI_Comm    MPI_Comm        /* in  */) {
    if (((datatype != MPI_INT)) || (operator != MPI_SUM)) {
        printf("[*]ERROR: We are a very lazy implementation and can only sum integers, sorry ;(");
        fflush(stdout);
        MPI_Abort(MPI_Comm, EXIT_FAILURE);
    }
    int comm_sz, rank;
    int recv[count];
    int* in = (int*)input_data_p;
    int* out = (int*)output_data_p;
    int send_count = 0;
    int recv_count = 0;
    MPI_Request srequest;
    MPI_Request rrequest;
    MPI_Comm_size(MPI_Comm, &comm_sz);
    MPI_Comm_rank(MPI_Comm, &rank);

    int offset = 1;
    while (offset <= (comm_sz / 2)) {
        int send = (rank + offset + comm_sz) % comm_sz;
        int receive = (rank - offset + comm_sz) % comm_sz;
        MPI_Isend(in, count, datatype, send,
                 0, MPI_Comm, &srequest);
        send_count += 1;
        MPI_Irecv(recv, count, datatype, receive,
                 0, MPI_Comm, &rrequest);
        recv_count += 1;
        MPI_Barrier(MPI_Comm);
        for (int i = 0; i < count; i++) {
            in[i] += recv[i];
            recv[i] = 0;
        }
        MPI_Barrier(MPI_Comm);
        offset = offset * 2;

    }
    MPI_Barrier(MPI_Comm);

    printf("process %d performed %d sends, %d receives\n", rank, send_count, recv_count);
    for (int i = 0; i < count; i++) {
        out[i] = in[i];
    }
    return EXIT_SUCCESS;
}


