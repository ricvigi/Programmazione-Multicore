#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#define ITERATIONS 10000000

void trapezoidal_rule();
static double square(double* x) {return *x**x;}
void trapezoidal_rule(int* a, double* h, double* res);
double local_trapezoidal(int* a, double* h);

int main(int argc, char** argv) {
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    double res = 0.0;
    double h = (double)(b - a) / ITERATIONS;

    /* ATTENTION: if we don't specify the reduction clause we
     * have a race condition */
#   pragma omp parallel reduction(+: res)
{
    res += local_trapezoidal(&a, &h);
}


    printf("Integral of x^2 between %d and %d is %f\n", a, b, res);
    return 0;
}

/* A version of the trapezoidal_rule that we can use to return a
 * value, instead of returning void. */
double local_trapezoidal(int* a, double* h) {
    int rank = omp_get_thread_num();
    int nthreads = omp_get_num_threads();
    int local_n = ITERATIONS / nthreads;
    double local_a = *a + rank*local_n**h;
    double local_b = local_a + local_n**h;

    double local_approx = (square(&local_a) + square(&local_b)) / 2;

    for (int i = local_a; i < local_n; i++) {
        double x_i = local_a + i**h;
        double t = square(&x_i);
        local_approx += t;
    }
    local_approx = *h * local_approx;
    return local_approx;
}


/* Computes the area under the curve between local_a and local_b,
 * then updates the global variable res in an atomic section */
void trapezoidal_rule(int* a, double* h, double* res) {
    int rank = omp_get_thread_num();
    int nthreads = omp_get_num_threads();
    int local_n = ITERATIONS / nthreads;
    double local_a = *a + rank*local_n**h;
    double local_b = local_a + local_n**h;

    double local_approx = (square(&local_a) + square(&local_b)) / 2;

    for (int i = local_a; i < local_n; i++) {
        double x_i = local_a + i**h;
        double t = square(&x_i);
        local_approx += t;
    }
    local_approx = *h * local_approx;


#   pragma omp atomic
    *res += local_approx;
}
