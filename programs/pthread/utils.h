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



#endif
