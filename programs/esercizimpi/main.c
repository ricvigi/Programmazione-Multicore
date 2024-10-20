#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
#include "exercises.h"

int main(int argc, char **argv) {
    int n = 16;
    int test[] = {14,3,1,6,9,99,123,4,2,6,123,4,32,12,12,19};
    serial_odd_even_sort(test, &n);
    for (int i  = 0; i < n; i++) {
        printf("%d\n", test[i]);
    }
    return 0;
}
