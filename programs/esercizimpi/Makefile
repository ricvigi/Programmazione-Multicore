# Targets and Dependencies
all: main

# Compile exercises.o
exercises.o: exercises.c exercises.h
	mpicc -g -Wall -c exercises.c -o exercises.o

# Compile main.o
main.o: main.c exercises.h
	mpicc -g -Wall -c main.c -o main.o

# Link main executable
main: main.o exercises.o
	mpicc -g -Wall -lm main.o exercises.o -o main

# Clean up
clean:
	rm -f *.o main
