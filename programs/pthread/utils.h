#include <sys/time.h>

#ifndef UTILS_H
#define UTILS_H

void* foo(void* a);
long* create_random_vector(long n);
long* create_zero_vector(long n);
void print_mat(long* A, long* m, long*n);
void* mat_vec_mul(void* rank);

static inline double square(double x) {
    return x*x;
}
static inline double cube(double x) {
    return x*x*x;
}

#define RANDOM_DOUBLE (-1 + 2 * ((double)rand() / RAND_MAX))
// #define GET_TIME(now) {
//     struct timeval t;
//     gettimeofday(&t, NULL);
//     now = t.tv_sec + t.tv_usec/1000000.0
// }
/* To use GET_TIME
 * double start, finish, elapsed;
 * GET_TIME(start);
 * ...
 * //CODE TO BE TIMED
 * ...
 * GET_TIME(finish);
 * elapsed = finish - start;
 * printf("It took: %e", elapsed);
 * */


#endif
