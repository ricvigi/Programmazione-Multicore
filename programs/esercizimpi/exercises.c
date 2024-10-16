#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
const double max = 100;
const int datacount = 16;

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

/* nummber_in_cicle = 0
 * for (toss = 0; toss < number_of_tosses; toss++) {
 * x = random double between -1 and 1;
 * y = random double between -1 and 1;
 * distance_squared = x*x + y*y;
 * if (distance_squared <= 1) {number_in_circle++}
 * }
 * pi_estimate = (4 * number_in_circle) / (double)number_of_tosses;
 */

double my_rand() {
    srand((unsigned int)time(NULL));
    return ((double)rand() / RAND_MAX) * max;
}

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
        double x = my_rand();
        double y = my_rand();
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
        double array0[datacount];
        double array1[datacount];
        for (int i = 0; i < datacount; i++) {
            array0[i] = 2.0;
            array1[i] = 1.0;
        }

        MPI_Scatter(array0, local_sz,
                    MPI_DOUBLE, dest0,
                    local_sz, MPI_DOUBLE,
                    0, MPI_COMM_WORLD);
        MPI_Scatter(array1, local_sz,
                    MPI_DOUBLE, dest1,
                    local_sz, MPI_DOUBLE,
                    0, MPI_COMM_WORLD);

        for (int i = 0; i < local_sz; i++) {
            res[i] = dest0[i] + dest1[i];
        }
        MPI_Gather(res, local_sz,
                   MPI_DOUBLE, gather,
                   local_sz, MPI_DOUBLE,
                   0, MPI_COMM_WORLD);
        for (int i = 0; i < datacount; i++) {
            printf("gather[%d]=%f\n", i, gather[i]);
        }

    } else {
        MPI_Scatter(NULL, local_sz,
                    MPI_DOUBLE, dest0,
                    local_sz, MPI_DOUBLE,
                    0, MPI_COMM_WORLD);
        MPI_Scatter(NULL, local_sz,
                    MPI_DOUBLE, dest1,
                    local_sz, MPI_DOUBLE,
                    0, MPI_COMM_WORLD);
        for (int i = 0; i < local_sz; i++) {
            res[i] = dest0[i] + dest1[i];
        }
        MPI_Gather(res, local_sz,
                   MPI_DOUBLE, gather,
                   local_sz, MPI_DOUBLE,
                   0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}


