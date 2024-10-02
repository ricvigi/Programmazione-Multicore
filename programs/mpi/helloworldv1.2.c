#include <stdio.h>
#include <mpi.h>

int main(void) {
  int comm_sz, my_rank;
  MPI_Init(NULL, NULL);
  //if (r != MPI_SUCCESS) {
  //MPI_Abort(MPI_COMM_WORLD, r);
  //}
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  if (my_rank == 0) {
     printf("[*] ZERO hello, world from process %d out of %d\n", my_rank, comm_sz);
     int i;
     for (i = 1; i < comm_sz; i++) {
       char str[256];
       MPI_Recv(str, 256, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       printf("ZERO received from %d, %s\n", i, str);
     }
  } else {
    char str[256];
    sprintf(str, "hello, world from process %d out of %d\n", my_rank, comm_sz);
    MPI_Send(str, 256, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  }
  //printf("hello, world from process %d out of %d\n", my_rank, comm_sz);
  MPI_Finalize();
  return 0;
}
