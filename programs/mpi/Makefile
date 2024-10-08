all: histogram_parallel.o main.o mpi_non_block.o 

mpi_non_block.o: mpi_non_block.c
	mpicc -g -Wall mpi_non_block.c -o mpi_non_block.o
helloworldv0.o: helloworldv0.c
	mpicc -g -Wall helloworldv0.c -o helloworldv0.o
helloworldv1.o: helloworldv1.c
	mpicc -g -Wall helloworldv1.c -o helloworldv1.o
helloworldv1_2.o: helloworldv1.2.c
	mpicc -g -Wall helloworldv1.2.c -o helloworldv1_2.o
histogram_parallel.o: histogram_parallel.c histogram.h
	mpicc -g -Wall -c histogram_parallel.c -o histogram_parallel.o
main.o: main.c histogram_parallel.o
	mpicc -g -Wall main.c histogram_parallel.o -o main.o
clean:
	rm -f *.o
