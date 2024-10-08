#include <stdio.h>
#include <mpi.h>

int main(void) {
  int rank, size;
  int send_right = 19;
  int send_left = 23;
  int recv_right, recv_left;

  
  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // int MPI_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)

  
  MPI_Request request_send_right, request_send_left;
  MPI_Request request_rcv_right, request_recv_left;
  MPI_Request requests[4];

  // Send to right. Isend wants a pointer to a request! &request_send_right in this case
  MPI_Isend(&send_right, 1, MPI_INT, ((rank + 1)%size), 0, MPI_COMM_WORLD, &requests[0]);
  // Send to left
  MPI_Isend(&send_left, 1, MPI_INT, ((rank - 1)%size), 0, MPI_COMM_WORLD, &requests[1]);
  // Recv from right
  MPI_Irecv(&recv_right, 1, MPI_INT, ((rank + 1)%size), MPI_COMM_WORLD, &requests[2]);
  // Recv from left
  MPI_Irecv(&recv_left, 1, MPI_INT, ((rank - 1)%size), MPI_COMM_WORLD, &requests[3]);

  MPI_Waitall(4, requests, MPI_STATUSES_IGNORE);
  
  MPI_Finalize();
  return 0;
}
