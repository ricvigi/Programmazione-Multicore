#include <stdio.h>
#include <mpi.h>

int main(void) {
  MPI_Init(NULL, NULL);
  printf("hello, world\n");
  MPI_Finalize();
  return 0;
}
// to run this with more threads than cores, use --oversubscribe with mpiexec.
// For example mpiexec --oversubscribe -n 20 yourprogram
