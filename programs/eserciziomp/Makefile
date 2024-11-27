all: eserciziomp.o main.o main

eserciziomp.o: eserciziomp.c eserciziomp.h
	gcc -g -Wall -fopenmp -c eserciziomp.c -o eserciziomp.o

main.o: main.c eserciziomp.h
	gcc -g -Wall -fopenmp -c main.c -o main.o

main: main.o eserciziomp.o
	gcc -g -Wall -fopenmp -lm main.o eserciziomp.o -o main

clean:
	rm *.o main
