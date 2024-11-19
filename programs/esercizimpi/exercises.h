#ifndef EXERCISES_H
#define EXERCISES_H

int random_double(int *nElements, const int *datacount,
                  double **array, const double *max);
int populate(double *array, int *nElements);
int approx_pi(int seed, int ntosses);
int trapezoidal_seq(double *a, double *b, double *n);
int trapezoidal_parallel(double *a, double *b, double *n);
int parallel_sum();
void serial_odd_even_sort(int *a, int *n);
void seq_matrix_matrix_mul(int* A,  /* in  */
                           int* B,  /* in  */
                           int* C,  /* out */
                           int* m,  /* in  */
                           int* n,  /* in  */
                           int* o,
                           int* p);
double my_rand(int seed);
int* create_random_vector(int n);
void print_mat(int* A, int* m, int* n);
void mat_vect_mult(int* A,
                   int* x,
                   int* y,
                   int* m,
                   int* n);
int parallel_mat_mat_mul(int* m, /* argv[1] */
                         int* n /* argv[2] */);
int MPI_Allreduce_custom(void*       input_data_p    /* in  */,
                         void*       output_data_p   /* out */,
                         int         count           /* in  */,
                         MPI_Datatype datatype       /* in  */,
                         MPI_Op      operator        /* in  */,
                         MPI_Comm    MPI_Comm        /* in  */);
int MPI_Allreduce_custom_optimized(
                         void*       input_data_p    /* in  */,
                         void*       output_data_p   /* out */,
                         int         count           /* in  */,
                         MPI_Datatype datatype       /* in  */,
                         MPI_Op      operator        /* in  */,
                         MPI_Comm    MPI_Comm        /* in  */);
int* seq_vec_sum(int* vec, int* res, int* s);
#endif
