all: main

main.o: main.c exercises.h
	gcc -g -Wall -lm -c main.c -o main.o -lpthread

exercises.o: exercises.c exercises.h
	gcc -g -Wall -lm -c exercises.c -o exercises.o -lpthread

main: main.o exercises.o
	gcc -g -Wall -lm main.o exercises.o -o main -lpthread
clean:
	rm *.o main
