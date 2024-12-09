#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "utils.h"

long* A;
long size;

int main(int argc, char** argv) {
    size = atoi(argv[1]);
    create_random_vector(A, size);

    return 0;
}
