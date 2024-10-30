#ifndef EXERCISES_H
#define EXERCISES_H

extern int random_double(int *nElements, const int *datacount, double **array, const double *max);
extern int populate(double *array, int *nElements);
extern void approx_pi(int seed, int ntosses);
extern int trapezoidal_seq(double *a, double *b, double *n);
extern int trapezoidal_parallel(double *a, double *b, double *n);
extern int parallel_sum();
void serial_odd_even_sort(int *a, int *n);
void seq_matrix_matrix_mul(int* A[][2],  /* in */
                           int* B[][2],  /* in */
                           int* C[][2],  /* out */
                           int* m,  /* in */
                           int* n   /* in */);

#endif
