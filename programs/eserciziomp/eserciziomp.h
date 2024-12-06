#include <sys/time.h>

#ifndef ESERCIZIOMP_H
#define ESERCIZIOMP_H
int Pi_mc();
int* create_random_vector(int n, int max);
void print_mat(int* A, int* m, int*n);

#define RANDOM_DOUBLE (-1 + 2 * ((double)rand() / RAND_MAX))
#define RANDOM (rand() % 10)
// #define GET_TIME(now) {
//     struct timeval t;
//     gettimeofday(&t, NULL);
//     now = t.tv_sec + t.tv_usec/1000000.o
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
