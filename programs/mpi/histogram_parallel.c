#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include "histogram.h"

// datacount stores how many elements are present in the array
// max contains the upper bound of our random numbers
const int datacount = 50;
const float max = 5.0;

const int bin_count = 5;
int histogram_sequential(){
  // comm_sz contains the total number of threads spawned
  // my_rank contains the rank of the thread executing
  int comm_sz, my_rank;

  // initialize MPI
  MPI_Init(NULL, NULL);

  // set Comm_size and Comm_rank
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  if (my_rank == 0) {
    // final histogram. 
    //int hist[bin_count];
    printf("[*]ZERO waiting for partial histograms\n");
  } else {
    //char str[256];
    printf("[*]Thread %d speaking\n", my_rank);
  }
  MPI_Finalize();
  return 0;
}


int populate(float *array) {
  // Initialize the array
  int nElements;

  // populates array variable with $datacount objects
  random_float(&nElements, &datacount, &array, &max);
  // remember to deallocate memory for the variable array.
  for (int i = 0; i < datacount; i++) {
    printf("%f ", array[i]);
  }
  free(array);
  return 0;
}

/* Dinamically populates an array (array) of floats with
   datacount elements. Elements of the array have a random
   value 0 <= x <= max */
int random_float(int *nElements,
		  const int *datacount,
		  float** array,
		  const float *max) {
  // initialize the seed for random number generation
  srand((unsigned int)time(NULL));

  
  for (int i=0;i<*datacount;i++) {
    // BE CAREFUL to use parenthesis in order to dereference
    // the pointer BEFORE upgrading it. ++ incremental has
    // priority over dereference!
    (*nElements)++;
    
    // the following expression returns a float 0 <= x <= max
    float t = ((float)rand()/(float)(RAND_MAX)) * *max;
    
    // dinamically allocate memory to the array
    *array = realloc(*array, sizeof(float)* *nElements);
    
    // check that allocation was succesful, otherwise return 1
    if (*array == NULL) {
      printf("[*]ERROR Memory allocation failed!\n");
      exit(1);
    }
    
    // populate the array with element t
    (*array)[*nElements - 1] = t;
  }
  return 0;
}
