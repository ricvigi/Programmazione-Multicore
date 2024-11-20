all: main

exercises.o: exercises.c exercises.h
	mpicc -g -Wall -c exercises.c -o exercises.o

main.o: main.c exercises.h
	mpicc -g -Wall -c main.c -o main.o

main: main.o exercises.o
	mpicc -g -Wall -lm main.o exercises.o -o main

clean:
	rm -f *.o main
