#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
const double max = 100;
const int datacount = 262144;

void print_mat(int* A, int* m, int*n) {
    for (int i = 0; i < *m; i++) {
        for (int j = 0; j < *n; j++) {
            printf("%d ", A[i * *n + j]);
        }
        puts("\n");
    }
}

/* Matrix-vector multiplication */
void mat_vect_mult(int* A,
                   int* x,
                   int* y,
                   int* m,
                   int* n){
    for (int i = 0; i < *m; i++) {
        y[i] = 0;
        for (int j = 0; j < *n; j++){
            y[i] += A[i * *n + j] * x[j];
        }
    }
}

int parallel_mat_mat_mul(  int* A,
                           int* B,
                           int* C,
                           int* m, /* Rows of A */
                           int* n, /* Columns of B */
                           int* o, /* Rows of B */
                           int* p /* Columns of A */) {


    return 0;
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
        printf("[*]HERE");
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

int* create_random_vector(int n){
    int* vec = (int*) malloc(n * sizeof(int));
    for(int i = 0; i < n; i++){
        vec[i] = rand() % 10;
    }
    return vec;
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
void approx_pi(int seed, int ntosses) {
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

    return 0;
}

int parallel_sum() {
    // a is needed by populate but maybe it will be
    // deleted...
    int rank, comm_sz;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    if (datacount % comm_sz != 0) {
        printf("[*]ERROR, array size should be divisible by number of processes\n");
        return 1;
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
    return 0;
}


