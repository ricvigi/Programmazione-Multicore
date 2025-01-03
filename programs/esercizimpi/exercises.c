#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
#include "exercises.h"

const double max = 100.0;
const int datacount = 262144;

/* At each iteration s we compute a new matrix A such that the
 * value in position (i,j) is the sum of the values in the
 * position (i-1,j), (i,j-1), (i+1,j), (i,j+1), i.e. perform a
 * iterative stencil. NOTE: Do this by allocating no more than
 * two matrices, regardless of the number of iterations you need.
 * Also, manage the case where the number of elements in the
 * matrix is larger than the number of processes. */
int parallel_iterative_stencil(int* m, int* n, int* K) {
    int rank, comm_sz;
    int l = 1;
    int s = *m * *n;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    /* For this implementation we require root to be 0 */
    const int root = 0;
    const int tail = comm_sz - 1;

    if (*m % comm_sz != 0) {
        printf("[*]ERROR, number of rows must be evenly divisible by number of processes\n");
        fflush(stdout);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    if (comm_sz >= s) {
        printf("[*]ERROR, you have too many processes\n");
        fflush(stdout);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    int local_sz = s / comm_sz;
    int scatter_recv[local_sz];
    int zero_scatter_recv[s];
    if (rank == root) {
        int* A = create_random_vector(s);
        print_mat(A, &l, &s);
        MPI_Scatter(A, local_sz,
                    MPI_INT, scatter_recv,
                    local_sz, MPI_INT,
                    root, MPI_COMM_WORLD);
        free(A);
    } else {
        MPI_Scatter(NULL, local_sz,
                    MPI_INT, scatter_recv,
                    local_sz, MPI_INT,
                    root, MPI_COMM_WORLD);
    }
    /* Receive buffers. Size is equal to the number of columns of
     * the matrix, since we are splitting over the rows */
    int recv0[*n];
    int recv1[*n];
    const int eol = *n - 1;
    for (int k = 0; k < *K; k++) {
        /* Code for root process */
        if (rank == root) {
            int dest = rank + 1;
            MPI_Sendrecv(scatter_recv, local_sz,
                         MPI_INT, dest, 0, recv0,
                         *n, MPI_INT, dest, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int i = 0; i < local_sz; i++) {
                if (i == 0) {
                    int DOWN = recv0[i];
                    int RIGHT = scatter_recv[i + 1];
                    zero_scatter_recv[i] = DOWN + RIGHT;
                }
                else if ( i == eol) {
                    int DOWN = recv0[i];
                    int LEFT = scatter_recv[i - 1];
                    zero_scatter_recv[i] = DOWN + LEFT;
                } else {
                    int DOWN = recv0[i];
                    int LEFT = scatter_recv[i - 1];
                    int RIGHT = scatter_recv[i + 1];
                    zero_scatter_recv[i] = DOWN + LEFT + RIGHT;
                }
            }
        } else if (rank == tail) { /* Code for tail process */
            int dest = rank - 1;
            MPI_Sendrecv(scatter_recv, local_sz,
                         MPI_INT, dest, 0, recv0,
                         local_sz, MPI_INT, dest,
                         0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int i = 0; i < local_sz; i++) {
                if (i == 0) {
                    int UP = recv0[i];
                    int RIGHT = scatter_recv[i + 1];
                    zero_scatter_recv[i] = UP + RIGHT;
                }
                else if ( i == eol) {
                    int UP = recv0[i];
                    int LEFT = scatter_recv[i - 1];
                    zero_scatter_recv[i] = UP + LEFT;
                } else {
                    int UP = recv0[i];
                    int LEFT = scatter_recv[i - 1];
                    int RIGHT = scatter_recv[i + 1];
                    zero_scatter_recv[i] = UP + LEFT + RIGHT;
                }
            }
        } else { /* Other processes */
            int dest0 = rank - 1;
            int dest1 = rank + 1;
            MPI_Sendrecv(scatter_recv, local_sz,
                         MPI_INT, dest0, 0, recv0,
                         local_sz, MPI_INT, dest0,
                         0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Sendrecv(scatter_recv, local_sz,
                         MPI_INT, dest1, 0, recv1,
                         local_sz, MPI_INT, dest1,
                         0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int i = 0; i < local_sz; i++) {
                if (i == 0) {
                    int UP = recv0[i];
                    int DOWN = recv1[i];
                    int RIGHT = scatter_recv[i + 1];
                    zero_scatter_recv[i] = UP + DOWN + RIGHT;
                }
                else if ( i == eol) {
                    int UP = recv0[i];
                    int DOWN = recv1[i];
                    int LEFT = scatter_recv[i - 1];
                    zero_scatter_recv[i] = UP + DOWN + LEFT;
                } else {
                    int UP = recv0[i];
                    int DOWN = recv1[i];
                    int LEFT = scatter_recv[i - 1];
                    int RIGHT = scatter_recv[i + 1];
                    zero_scatter_recv[i] = UP + DOWN + LEFT + RIGHT;
                }
            }
        }
        for (int i = 0; i < local_sz; i++) {
            scatter_recv[i] = zero_scatter_recv[i];
            zero_scatter_recv[i] = 0;
        }
    }
    MPI_Gather(scatter_recv, local_sz, MPI_INT,
               zero_scatter_recv, local_sz, MPI_INT,
               root, MPI_COMM_WORLD);
    if (rank == root) {
        print_mat(zero_scatter_recv, m, n);
    }
    MPI_Finalize();
    return EXIT_SUCCESS;
}

/* Add all elements of a vector */
int seq_vec_sum(int* vec, int* res, int* s) {
    for (int i = 0; i < *s; i++) {
        *res += vec[i];
    }
    return 0;
}

void print_mat(int* A, int* m, int*n) {
    for (int i = 0; i < *m; i++) {
        for (int j = 0; j < *n; j++) {
            printf("%d ", A[i * *n + j]);
        }
        puts("\n");
    }
}

/* main function for Allreduce implementation */
int Allreduce_main(int argc, char** argv) {
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

/* Custom implementation of MPI_Allreduce. Not very efficient
 * NOTE: Change this function's sends and receives to send more
 * than one element at a time. Send the whole buffer instead to
 * reduce the number of sends. */
int MPI_Allreduce_custom(
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
    int root = 0;
    int* in = (int*)input_data_p;
    int* out = (int*)output_data_p;
    int count_send = 0;
    int count_recv = 0;
    MPI_Comm_size(MPI_Comm, &comm_sz);
    MPI_Comm_rank(MPI_Comm, &rank);


    if (rank != root) {
        for (int k = 0; k < count; k++) {
            MPI_Send(&in[k], 1, datatype, root,
                    0, MPI_Comm);
            count_send += 1;
        }
    }

    if (rank == root) {
        /* receive from other processes and sum with root's input */
        for (int j = 1; j < comm_sz; j++) {
            for (int l = 0; l < count; l++){
                MPI_Recv(&recv[l], 1, datatype, j, 0,
                         MPI_Comm, MPI_STATUS_IGNORE);
                count_recv += 1;
                in[l] += recv[l];
            }
        }
        /* Send to all other processes, including root */
        for (int l = 0; l < comm_sz; l++) {
            for (int i = 0; i < count; i++) {
                MPI_Send(&in[i], 1, datatype, l,
                         0, MPI_Comm);
                count_send += 1;
            }
        }
    }
    /* Receive result into out buffer */
    MPI_Barrier(MPI_Comm);
    for (int i = 0; i < count; i++) {
        MPI_Recv(&out[i], 1, datatype, root, 0,
                 MPI_Comm, MPI_STATUS_IGNORE);
        count_recv += 1;
    }

    printf("process %d performed %d sends, %d receives\n", rank, count_send, count_recv);
    return EXIT_SUCCESS;
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

    /* Initial offset is 1. The algorithm ends when offset is ==
     * to the number of processes / 2 (otherwise we would be
     * adding each local buffer with itself!) */
    int offset = 1;
    while (offset <= (comm_sz / 2)) {
        /* Here we compute send and receive processes for ith
         * iteration.
         * Although we do not implement directly the "butterfly"
         * pattern, doubling the distance from the send process
         * at each iteration still works. At the final
         * iteration, the algorithm will have (magically)
         * distributed the correct sum to all other vectors,
         * going through different values for each process. */
        int send = (rank + offset + comm_sz) % comm_sz;
        int receive = (rank - offset + comm_sz) % comm_sz;

        /* Perform send and receive. NOTE: This can also be done
         * through MPI_Sendrecv */
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
/* Creates a random vector of integers of size n. The integers are in the
 * range [0,9]. Change the modulo in vec[i] = rand() % k to change the range */
int* create_random_vector(int n) {
    int* vec = (int*) malloc(n * sizeof(int));
    for(int i = 0; i < n; i++){
        vec[i] = rand() % 10;
    }
    return vec;
}

/* Creates a zeroed vector of integers of size n */
int* create_zero_vector(int n) {
    int* vec = (int*) malloc(n*sizeof(int));
    for (int i = 0; i < n; i++) {
        vec[i] = 0;
    }
    return vec;
}


/* Matrix-vector multiplication with
 * column major order */
void mat_vect_mult(int* A,
                   int* x,
                   int* y,
                   int* m,
                   int* n) {
    for (int j = 0; j < *n; j++) {
        y[j] = 0;
        for (int i = 0; i < *m; i++){
            y[j] += A[i * *n + j] * x[i];
        }
    }
}

/* Parallel implementation of matrix-matrix multiplication. This
 * particular implementation only works on square matrices and
 * the necessary condition for the parallelization to work is
 * that the number of threads must be equal to the number of
 * rows of the two matrices, i.e. for 4x4 matrices we need 4
 * threads, for 5x5 5 threads, etc... */
int parallel_mat_mat_mul(int* m, /* argv[1] */
                         int* n  /* argv[2] */) {
    int p = (*m * *n);
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
    if (*m != *n) {
        printf("[*]ERROR, we are only dealing with square matrices at the moment\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    const int local_sz = p / comm_sz;
    const int root = comm_sz - 1;

    /* destination of SCATTERED matrix */
    int dest[local_sz];

    /* destination of BROADCASTED matrix */
    int dest1[p];

    /* local result array */
    int res[local_sz];

    /* Result matrix to store the result of the multiplication. */
    int C[p];

    /* rank p - 1 should generate the matrices and
     * scatter them */
    if (rank == root) {
        int* A = create_random_vector(p);
        int* B = create_random_vector(p);

        /* Place a barrier before scatter operation */
        MPI_Barrier(MPI_COMM_WORLD);

        /* Scatter A matrix */
        MPI_Scatter(A, local_sz,
                    MPI_INT, dest,
                    local_sz, MPI_INT,
                    root, MPI_COMM_WORLD);

        /* Broadcast B matrix */
        MPI_Bcast (B, p, MPI_INT, root, MPI_COMM_WORLD);

        mat_vect_mult(B, &dest[0], &res[0], m, n);

        /* Gather call to store all results into C */
        MPI_Gather(res, local_sz,
                   MPI_INT, C,
                   local_sz, MPI_INT,
                   0, MPI_COMM_WORLD);

        MPI_Barrier(MPI_COMM_WORLD);
        free(A);
        free(B);
    }
    else {
        /* Place a barrier before scatter operation */
        MPI_Barrier(MPI_COMM_WORLD);

        /* Receive scattered A matrix into dest */
        MPI_Scatter(NULL, local_sz,
                    MPI_INT, dest,
                    local_sz, MPI_INT,
                    root, MPI_COMM_WORLD);

        /* Receive broadcasted B matrix into dest1 */
        MPI_Bcast (dest1, p, MPI_INT, root, MPI_COMM_WORLD);

        /* Perform local computation */
        mat_vect_mult(&dest1[0], &dest[0], &res[0], m, n);

        /* Gather call */
        MPI_Gather(res, local_sz,
                    MPI_INT, C,
                    local_sz, MPI_INT,
                    0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
    }

    if (rank == 0) {
        print_mat(&C[0], m, n);
    }
    MPI_Finalize();
    return EXIT_SUCCESS;
}

int seq_matrix_matrix_mul( int* A,
                           int* B,
                           int* C,
                           int* m, /* Rows of A */
                           int* n, /* Columns of B */
                           int* o, /* Rows of B */
                           int* p /* Columns of A */) {
    if (*p != *o) {
        printf("[*]Error: Columns of A (%d) and Rows of B (%d) are different!\n", *p, *m);
        return 1;
    }
    int c;

    for (int i = 0; i < *m; i++) {
        for (int j = 0; j < *n; j++) {
            c = 0;
            for (int k = 0; k < *o; k++) {
                c += A[i * *n + k] * B[k * *n + j];
            }
            C[i * *n + j] = c;
        }
    }
    return 0;
}


/* Serial implementation of odd-even transposition sort */
void serial_odd_even_sort(int *a, int *n) {
    for (int phase = 0; phase < *n; phase++) {
        if (phase % 2 == 0) { /* Even phase */
            for (int i = 1; i < *n; i += 2) {
                if (a[i - 1] > a[i]) {
                    int t = a[i];
                    a[i] = a[i-1];
                    a[i-1] = t;
                }
            }
        } else { /* Odd phase */
            for (int j = 1; j < *n-1; j += 2) {
                if (a[j] > a[j+1]) {
                    int t = a[j];
                    a[j] = a[j+1];
                    a[j+1] = t;
                }
            }
        }
    }
}

/* Dinamically populates an array (array) of doubles with
   datacount elements. Elements of the array have a random
   value 0 <= x <= max */
int random_double(int *nElements,
                  const int *datacount,
                  double **array,
                  const double *max) {
    // initialize the seed for random number generation
    srand((unsigned int)time(NULL));

    for (int i = 0; i < *datacount; i++) {
        // BE CAREFUL to use parenthesis in order to dereference
        // the pointer BEFORE upgrading it. ++ incremental has
        // priority over dereference!
        (*nElements)++;

        // the following expression returns a double 0 <= x <= max
        double t = ((double)rand() / (RAND_MAX)) * *max;

        // dinamically allocate memory to the array
        *array = realloc(*array, sizeof(double) * *nElements);

        // statically populate the vector. nElements in this
        // case is not needed, but we keep it in the function
        // so that we can switch from static to dynamic...
        // (*array)[i] = t;
        // check that allocation was succesful, otherwise return 1
        if (*array == NULL) {
            printf("[*]ERROR Memory allocation failed!\n");
            exit(1);
        }

        // populate the array with element t
        (*array)[*nElements - 1] = t;
    }
    return 0;
}

int populate(double *array, int *nElements) {
    // populates array variable with $datacount objects
    random_double(nElements, &datacount, &array, &max);

    // remember to deallocate memory for the variable array.
    for (int i = 0; i < datacount; i++) {
        printf("%f ", array[i]);
    }
    printf("\n");
    // free(array);
    return 0;
}

double my_rand(int seed) {
    srand(seed);
    return ((double)rand() / RAND_MAX) * max;
}

/* number_in_cicle = 0
 * for (toss = 0; toss < number_of_tosses; toss++) {
 * x = random double between -1 and 1;
 * y = random double between -1 and 1;
 * distance_squared = x*x + y*y;
 * if (distance_squared <= 1) {number_in_circle++}
 * }
 * pi_estimate = (4 * number_in_circle) / (double)number_of_tosses;
 */
/* This is a parallel program that computes
 * an approximation of pi */
int approx_pi(int seed, int ntosses) {
    double start = MPI_Wtime();
    int size, rank, num_in_circle;
    int i;
    num_in_circle = 0;
    /* set the seed value for random number generation */
    srand(seed);

    MPI_Init(NULL, NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int num_local_tosses = ntosses / size;
    int local_num_in_circle = 0;

    for (i = 0; i < num_local_tosses; i++) {
        double x = my_rand(999);
        double y = my_rand(888);
        if ((x * x) + (y * y) <= 1) {
            local_num_in_circle++;
        }
    }
    MPI_Reduce(&local_num_in_circle, &num_in_circle, 1,
               MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        double pi = ((double)num_in_circle * 4) / ntosses;
        printf("pi is: %f\n", pi);
        printf("finished in: %f\n", MPI_Wtime() - start);
    }
    MPI_Finalize();
    return EXIT_SUCCESS;
}

/* Pseudo code for serial trapezoidal rule
 * f(x) = x^2
 * Input: a, b, n
 * h = (b - a) / n;
 * approx = (f(a) + f(b)) / 2.0;
 * for (i = 1; i <= n-1; i++) {
 *   x_i = a + (i*h);
 *   approx += f(x_i);
 * }
 * approx = h * approx;
 */
int trapezoidal_seq(double *a, double *b, double *n) {
    double h = (*b - *a) / *n;
    double approx = (pow(*a, 2.0) + pow(*b, 2.0)) / 2.0;
    for (int i = 1; i <= *n - 1; i++) {
        double t = *a + (i * h);
        approx += pow(t, 2.0);
    }
    approx = h * approx;
    printf("%f\n", approx);
    return 0;
}

int trapezoidal_parallel(double *a, double *b, double *n) {
    double start = MPI_Wtime();
    int size, rank;
    double total = 0.0;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double h = (*b - *a) / *n;
    double local_n = *n / size;
    double local_a = *a + rank * local_n * h;
    double local_b = local_a + local_n * h;
    double local_approx = (pow(local_a, 2.0) + pow(local_b, 2.0)) / 2.0;
    for (int i = 1; i <= local_n - 1; i++) {
        double t = local_a + (i * h);
        local_approx += pow(t, 2.0);
    }
    local_approx = h * local_approx;
    MPI_Reduce(&local_approx, &total, 1,
               MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        printf("Area is: %f\n", total);
        double finish = MPI_Wtime();
        printf("finished in: %f\n", finish - start);
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}

int parallel_sum() {
    int rank, comm_sz;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    if (datacount % comm_sz != 0) {
        printf("[*]ERROR, array size should be divisible by number of processes\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    int local_sz = datacount / comm_sz;
    double gather[datacount];
    double res[local_sz];
    double dest0[local_sz];
    double dest1[local_sz];
    if (rank == 0) {
        /* Populate the arrays */
        double array0[datacount];
        double array1[datacount];
        for (int i = 0; i < datacount; i++) {
            array0[i] = my_rand(1233 + i);
        }
        for (int i = 0; i < datacount; i++) {
            array1[i] = my_rand(999 + i);
        }

        /* Check the values */
        for (int i = 0; i < datacount; i++) {
            if (i % 4096 == 0) {
                printf("array0[%d] = %f \t array1[%d] = %f\n", i, array0[i], i, array1[i]);
            }
        }

        /* Scatter the first array */
        MPI_Scatter(array0, local_sz,
                    MPI_DOUBLE, dest0,
                    local_sz, MPI_DOUBLE,
                    0, MPI_COMM_WORLD);
        /* Scatter the second array */
        MPI_Scatter(array1, local_sz,
                    MPI_DOUBLE, dest1,
                    local_sz, MPI_DOUBLE,
                    0, MPI_COMM_WORLD);
        /* Perform 0's side of addition */
        for (int i = 0; i < local_sz; i++) {
            res[i] = dest0[i] + dest1[i];
        }
        /* Call gather from 0's side to receive into gather array */
        MPI_Gather(res, local_sz,
                   MPI_DOUBLE, gather,
                   local_sz, MPI_DOUBLE,
                   0, MPI_COMM_WORLD);
        for (int i = 0; i < datacount; i++) {
            if (i % 4096 == 0) {
                printf("gather[%d]=%f\n", i, gather[i]);
            }
        }

    } else {

        /* Scatter call for non 0 processes, stores values of
         * array0 into dest0 and array1 into dest1*/
        MPI_Scatter(NULL, local_sz,
                    MPI_DOUBLE, dest0,
                    local_sz, MPI_DOUBLE,
                    0, MPI_COMM_WORLD);
        MPI_Scatter(NULL, local_sz,
                    MPI_DOUBLE, dest1,
                    local_sz, MPI_DOUBLE,
                    0, MPI_COMM_WORLD);

        /* Perform addition */
        for (int i = 0; i < local_sz; i++) {
            res[i] = dest0[i] + dest1[i];
        }

        /* Gather call to send values stored in res into gather array */
        MPI_Gather(res, local_sz,
                   MPI_DOUBLE, gather,
                   local_sz, MPI_DOUBLE,
                   0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}


