#ifndef EXERCISES_H
#define EXERCISES_H

extern int random_double(int *nElements, const int *datacount, double **array, const double *max);
extern int populate(double *array, int *nElements);
extern void approx_pi(int seed, int ntosses);
extern int trapezoidal_seq(double *a, double *b, double *n);
extern int trapezoidal_parallel(double *a, double *b, double *n);
extern int parallel_sum();

#endif
