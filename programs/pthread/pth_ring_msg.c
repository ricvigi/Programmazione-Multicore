#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <semaphore.h>

#define MSG_MAX 100

int nthreads;
void* send_msg(void* rank);
void* sem_send_msg(void* rank);
char** messages;
sem_t* semaphore;

int main(int argc, char** argv) {
    nthreads = atoi(argv[1]);
    pthread_t* handles;
    handles = malloc(nthreads*sizeof(pthread_t));
    messages = malloc(nthreads*sizeof(char**));
    semaphore = malloc(nthreads*sizeof(sem_t));

    for (long rank = 0; rank < nthreads; rank++) {
        pthread_create(&handles[rank], NULL, sem_send_msg, (void*) rank);
    }

    for (int rank = 0; rank < nthreads; rank++) {
        pthread_join(handles[rank], NULL);
    }
    free(handles);
    free(messages);
    free(semaphore);
    return 0;
}


/* This version of send_msg has a producer/consumer issue. Some
 * threads might finish execution without having received
 * anything from their neighbour Thread */
void* send_msg(void* rank) {
    long r = (long) rank;
    long dest = (r + 1) % nthreads;
    long source = (r + nthreads - 1) % nthreads;
    char* msg = malloc(MSG_MAX*sizeof(char));

    sprintf(msg, "Hello to %ld from %ld\n", dest, r);
    messages[dest] = msg;

    if (messages[r] != NULL)
        printf("Thread %ld > %s\n", r, messages[r]);
    else
        printf("Thread %ld > No message from %ld", r, source);

    free(msg);
    return NULL;
}

/* This doesn't really work and i'm not sure why. */
void* sem_send_msg(void* rank) {
    long r = (long) rank;
    long dest = (r + 1) % nthreads;
    char* msg = malloc(MSG_MAX*sizeof(char));

    sprintf(msg, "Hello to %ld from %ld\n", dest, r);
    messages[dest] = msg;
    /* "Unlock" the semaphore of dest */
    sem_post(&semaphore[dest]);

    /* Wait of our semaphore to be unlocked */
    sem_wait(&semaphore[r]);
    printf("Thread %ld > %s\n", r, messages[r]);
    fflush(stdout);
    free(msg);
    return NULL;
}
