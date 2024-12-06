#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#define ITERATIONS 10000000

void trapezoidal_rule();
static double square(double* x) {return *x**x;}
void trapezoidal_rule(int* a, double* h, double* res);

int main(int argc, char** argv) {
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    double res = 0.0;
    double h = (double)(b - a) / ITERATIONS;
#   pragma omp parallel
{
    trapezoidal_rule(&a, &h, &res);
}
    printf("Integral of x^2 between %d and %d is %f\n", a, b, res);
    return 0;
}

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
    fflush(stdout);

#   pragma omp atomic
    *res += local_approx;
}
