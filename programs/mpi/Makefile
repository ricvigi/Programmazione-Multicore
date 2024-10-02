all: helloworldv0 helloworldv1 helloworldv1_2 

helloworldv0: helloworldv0.c
	mpicc -g -Wall helloworldv0.c -o helloworldv0
helloworldv1: helloworldv1.c
	mpicc -g -Wall helloworldv1.c -o helloworldv1
helloworldv1_2: helloworldv1.2.c
	mpicc -g -Wall helloworldv1.2.c -o helloworldv1_2
clean:
	rm -f helloworldv0 helloworldv1 helloworldv1_2
