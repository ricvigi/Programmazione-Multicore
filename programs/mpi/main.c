#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "histogram.h"


int main(void) {
  histogram_sequential();
  populate();
  return 0;
}
