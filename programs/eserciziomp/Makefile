all: omp_trapezoidal.o

eserciziomp.o: eserciziomp.c eserciziomp.h
	gcc -g -Wall -fopenmp -c eserciziomp.c -o eserciziomp.o

omp_trapezoidal.o: omp_trapezoidal.c
	gcc -g -Wall -fopenmp omp_trapezoidal.c -o omp_trapezoidal.o

hello.o: hello.c eserciziomp.h
	gcc -g -Wall -fopenmp -c hello.c -o hello.o

hello: hello.o eserciziomp.o
	gcc -g -Wall -fopenmp hello.o eserciziomp.o -o hello

clean:
	rm *.o hello
