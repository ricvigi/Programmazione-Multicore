#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include "histogram.h"

// datacount stores how many elements are present in the array
const int datacount = 50;

// max value
const float max = 5.0;
int populate() {
  // Initialize the array
  float *array = NULL;
  int nElements = 0;

  // populates array variable with $datacount objects
  random_float(&nElements, &datacount, &array, &max);
  // remember to deallocate memory for the variable array.
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
