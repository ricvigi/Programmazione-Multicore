all: eserciziomp.o hello.o hello

eserciziomp.o: eserciziomp.c eserciziomp.h
	gcc -g -Wall -fopenmp -c eserciziomp.c -o eserciziomp.o

hello.o: hello.c eserciziomp.h
	gcc -g -Wall -fopenmp -c hello.c -o hello.o

hello: hello.o eserciziomp.o
	gcc -g -Wall -fopenmp hello.o eserciziomp.o -o hello

clean:
	rm *.o hello
