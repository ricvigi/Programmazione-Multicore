#include <stdio.h>
#include <pthread.h>
#include <math.h>

void parallel_approx(long *n);


void serial_approxpi(long *n) {
    double pi = 0.0;
    for (int i = 0; i < *n; i++) {
        pi += (pow(-1, i))/(2*i + 1);
    }
    pi = 4 * pi;
    printf("%f\n", pi);
}



int main(int argc, char** argv) {
    long n = 99999999;
    serial_approxpi(&n);

    return 0;
}
