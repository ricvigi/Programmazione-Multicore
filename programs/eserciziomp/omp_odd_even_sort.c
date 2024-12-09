#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "eserciziomp.h"

#define MAX 10000

void omp_odd_even_sort(int* A, int n)
{
    int i, tmp, phase;

#   pragma omp parallel default(none) shared(A, n) private(i, tmp, phase)
{
    int nthreads = omp_get_num_threads();
    printf("nthreads = %d\n", nthreads);
    for (phase = 0; phase < n; phase++) {
        if (phase % 2 == 0)
#           pragma omp for
            for (i = 1; i < n; i += 2) {
                if (A[i - 1] > A[i]) {
                    tmp = A[i - 1];
                    A[i - 1] = A[i];
                    A[i] = tmp;
                }
            }
        else
#           pragma omp for
            for (i = 1; i < n - 1; i += 2) {
                if (A[i] > A[i + 1]) {
                    tmp = A[i + 1];
                    A[i + 1] = A[i];
                    A[i] = tmp;
                }
            }
        }
}
}

int main(int argc, char** argv) {
    double start, finish, tot;
    int n = atoi(argv[1]);
    int i = 1;
    int* A = create_random_vector(n, MAX);
    start = omp_get_wtime();
    omp_odd_even_sort(A, n);
    finish = omp_get_wtime();
    tot = finish - start;
    print_mat(A, &i, &n);
    printf("Total time: %f\n", tot);


    return 0;
}
