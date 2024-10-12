#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

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

/* This is a parallel program that computes
 * an approximation of pi*/
void approx_pi(int seed, int ntosses) {
  int size, rank, num_in_circle;
  int i;
  num_in_circle = 0;
  /* set the seed value for random number generation */
  srand(seed);
  
  MPI_Init(NULL,NULL);
  double time = MPI_Wtime();
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  int num_local_tosses = ntosses / size;
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
    double pi = ((double)num_in_circle * 4)/ntosses;
    double now = MPI_Wtime();
    printf("%f %f\n", pi, now - time);
  }
  MPI_Finalize();
}

/* Pseudo code for serial trapezoidal rule
 * f(x) = x^2
 * Input: a, b, n
 * h = (b - a) / n;
 * approx = (f(a) + f(b)) / 2.0;
 * for (i = 1; i <= n-1; i++) {
 *   x_i = a + (i*h);
 *   approx += f(x_i);
 * }
 * approx = h*approx;
 */
int trapezoidal_seq(double *a, double *b, double *n) {
  double h = (*b - *a) / *n;
  double approx = (pow(*a, 2.0) + pow(*b, 2.0)) / 2.0;
  for (int i = 1; i <= *n - 1; i++) {
    double t = *a + (i * h);
    approx += pow(t, 2.0);
  }
  approx = h * approx;
  printf("%f\n", approx);
  return 0;
}
int trapezoidal_parallel(double *a, double *b, double *n) {

  int size, rank;
  double total = 0.0;
  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  double h = (*b - *a) / *n;
  double local_n = *n / size;
  double local_a = *a + rank*local_n*h;
  double local_b = local_a + local_n*h;
  double local_approx = (pow(local_a, 2.0) + pow(local_b, 2.0)) / 2.0;
  for (int i = 1; i <= local_n - 1; i++){
    double t = local_a + (i * h);
    local_approx += pow(t, 2.0);
  }
  local_approx = h * local_approx;
  MPI_Reduce(&local_approx, &total, 1,
	     MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  if (rank == 0) {
    printf("%f\n", total);
  }

  MPI_Finalize();
  return 0;
}
int main(int argc, char **argv) {
  double a = (double)atoi(argv[1]);
  double b = (double)atoi(argv[2]);
  double n =  (double)atoi(argv[3]);
  trapezoidal_parallel(&a, &b, &n);
  return 0;
}
