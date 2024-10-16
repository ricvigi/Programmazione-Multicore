all: main.o

main.o: main.c exercises.h
	mpicc -g -Wall -lm main.c -o main.o

exercises.o: exercises.c exercises.h
	mpicc -g -Wall -lm -c exercises.c -o exercises.o
clean:
	rm -f *.o main
