#include <stdio.h>
#include "esercizi.h"

int esercizio_struct1() {
  Automobile *array;
  int nElements = 0;
  for (int i = 0; i < 3; i++) {
    nElements++;
    array = realloc(array, sizeof(Automobile)*nElements);
    if (array==NULL) {
      printf("[*]ERROR: No more memory available :(\n");
      exit(1);
    }
  }
}
