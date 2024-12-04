all: pth_mat_vec_mul pth_approx_pi

pth_approx_pi.o: pth_approx_pi.c utils.h
	gcc -g -Wall -lm -c pth_approx_pi.c -o pth_approx_pi.o -lpthread

pth_mat_vec_mul.o: pth_mat_vec_mul.c utils.h
	gcc -g -Wall -lm -c pth_mat_vec_mul.c -o pth_mat_vec_mul.o -lpthread

utils.o: utils.c utils.h
	gcc -g -Wall -lm -c utils.c -o utils.o -lpthread

pth_mat_vec_mul: pth_mat_vec_mul.o utils.o
	gcc -g -Wall -lm pth_mat_vec_mul.o utils.o -o pth_mat_vec_mul -lpthread

pth_approx_pi: pth_approx_pi.o utils.o
	gcc -g -Wall -lm pth_approx_pi.o utils.o -o pth_approx_pi -lpthread
clean:
	rm *.o pth_mat_vec_mul pth_approx_pi
