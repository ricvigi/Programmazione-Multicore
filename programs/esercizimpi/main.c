#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


/* nummber_in_cicle = 0
 * for (toss = 0; toss < number_of_tosses; toss++) {
 * x = random double between -1 and 1;
 * y = random double between -1 and 1;
 * distance_squared = x*x + y*y;
 * if (distance_squared <= 1) {number_in_circle++}
 * }
 * pi_estimate = (4 * number_in_circle)/(double)number_of_tosses;
 */

double my_rand() {
  return ((double)rand() / RAND_MAX)*2.0 - 1.0;
}


int main(int argc, char **argv) {
  int size, rank, num_of_tosses, num_in_circle;
  int i;
  num_in_circle = 0;
  srand(atoi(argv[1]));
  num_of_tosses = atoi(argv[2]);
  MPI_Init(NULL,NULL);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  int num_local_tosses = num_of_tosses / size;
  int local_num_in_circle = 0;
  
  for (i = 0; i < num_local_tosses; i++) {
    double x = my_rand();
    double y = my_rand();
    if ((x * x) + (y * y) <= 1) {
      local_num_in_circle++;
    }
  }
  MPI_Reduce(&local_num_in_circle, &num_in_circle, 1,
	     MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  if (rank == 0) {
    double pi = ((double)num_in_circle * 4)/num_of_tosses;
    printf("%f\n", pi);
  }
  MPI_Finalize();
  return 0;
}
