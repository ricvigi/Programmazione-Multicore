all: helloworldv0.o helloworldv1.o helloworldv1_2.o histogram_parallel.o main.o

helloworldv0.o: helloworldv0.c
	mpicc -g -Wall helloworldv0.c -o helloworldv0.o
helloworldv1.o: helloworldv1.c
	mpicc -g -Wall helloworldv1.c -o helloworldv1.o
helloworldv1_2.o: helloworldv1.2.c
	mpicc -g -Wall helloworldv1.2.c -o helloworldv1_2.o
histogram_parallel.o: histogram_parallel.c histogram.h
	mpicc -g -Wall histogram_parallel.c -o histogram_parallel.o
main.o: main.c hisogram_parallel.c
	mpicc -g -Wall main.c -o main.o
clean:
	rm -f *.o
