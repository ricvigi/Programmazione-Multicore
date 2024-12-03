#include <stdio.h>
#include <pthread.h>
#include <math.h>

void* foo(void* a) {
    long* px = (long*) a;
    long x = *px;
    printf("x=%ld\n", x);
    fflush(stdout);
    return NULL;
}
