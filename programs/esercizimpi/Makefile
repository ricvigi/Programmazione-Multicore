all: exercises.o main.o

exercises.o: exercises.c exercises.h
	mpicc -g -Wall -lm -c exercises.c -o exercises.o

main.o: main.c exercises.h
	mpicc -g -Wall -lm main.c exercises.o -o main.o

clean:
	rm -f *.o main
