#include <stdio.h>
#include <stdlib.h>
#include "esercizi.h"

int esercizio_malloc2() {
  int *array;
  int nElements = 0;
  while (1) {
    int in = 0;
    printf("Insert number (0 to terminate): ");
    scanf("%d", &in);
    if (in == 0) {
      break;
    } // break the loop if 0 is inserted or nothing is inserted
    nElements++;
    array = realloc(array, (sizeof(int)*nElements));
    if (array==NULL) {
      printf("[*]ERROR: No more memory available :(\n");
      exit(1);
    }
    array[nElements - 1] = in;
  }
  int min = array[0];
  int max = array[0];
  for (int i = 0; i < nElements; i++) {
    if (array[i] < min) {min = array[i];}
    if (array[i] > max) {max = array[i];}
    }
  printf("min: %d, max: %d\n", min, max);
  free(array);
  printf("[*] Don't forget to free ;)\n");
  return 0;
  }

int esercizio_malloc1() {
  int *array; // array of integers
  int nElements = 0;
  while(1) {
    int in = 0;
    printf("[*]Insert your number (0 to terminate): ");
    scanf("%d", &in);
    if (in == 0) {
      break;
    } // break the loop if 0 is inserted or nothing is inserted
    nElements++;
    array = realloc(array, (sizeof(int)*nElements));
    if (array==NULL) {
      printf("[*]ERROR: No more memory available :(\n");
      exit(1);
    }
    array[nElements - 1] = in;
  }
  int sum = 0;
  for (int i = 0; i < nElements; i++) {
    sum += array[i];
  }
  printf("[*]Sum: %d\n", sum);
  free(array);
  printf("[*]Don't forget to free() ;)\n");
  return 0;
}
