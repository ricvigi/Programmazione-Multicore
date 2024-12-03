#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "exercises.h"


int main(int argc, char** argv) {
    int n = atoi(argv[1]);
    pthread_t* handles;
    handles = malloc(n*sizeof(pthread_t));
    long x = 2;

    for (int thread = 0; thread < n; thread++) {
        pthread_create(&handles[thread], NULL, foo, (void*) &x);
    }

    printf("Hi from main thread\n");
    fflush(stdout);

    for (int thread = 0; thread < n; thread++) {
        pthread_join(handles[thread], NULL);
    }
    free(handles);
    return 0;
}
