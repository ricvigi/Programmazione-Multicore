all:pth_approx_pi pth_trapezoidal_rule pth_approx_pi.o pth_trapezoidal_rule.o  utils.o

pth_ring_msg.o: pth_ring_msg.c utils.h
	gcc -g -Wall -lm -c pth_ring_msg.c -o pth_ring_msg.o -lpthread

pth_approx_pi.o: pth_approx_pi.c utils.h
	gcc -g -Wall -lm -c pth_approx_pi.c -o pth_approx_pi.o -lpthread

pth_trapezoidal_rule.o: pth_trapezoidal_rule.c utils.h
	gcc -g -Wall -lm -lpthread -c pth_trapezoidal_rule.c -o pth_trapezoidal_rule.o

pth_approx_pi_circle.o: pth_approx_pi_circle.c utils.h
	gcc -g -Wall -lm -c pth_approx_pi_circle.c -o pth_approx_pi_circle.o -lpthread

pth_mat_vec_mul.o: pth_mat_vec_mul.c utils.h
	gcc -g -Wall -lm -c pth_mat_vec_mul.c -o pth_mat_vec_mul.o -lpthread

utils.o: utils.c utils.h
	gcc -g -Wall -lm -c utils.c -o utils.o -lpthread

pth_mat_vec_mul: pth_mat_vec_mul.o utils.o
	gcc -g -Wall -lm pth_mat_vec_mul.o utils.o -o pth_mat_vec_mul -lpthread

pth_approx_pi: pth_approx_pi.o utils.o
	gcc -g -Wall -lm pth_approx_pi.o utils.o -o pth_approx_pi -lpthread

pth_approx_pi_circle: pth_approx_pi_circle.o utils.o
	gcc -g -Wall -lm pth_approx_pi_circle.o utils.o -o pth_approx_pi_circle -lpthread

pth_ring_msg: pth_ring_msg.o utils.o
	gcc -g -Wall -lm pth_ring_msg.o utils.o -o pth_ring_msg -lpthread

pth_trapezoidal_rule: pth_trapezoidal_rule.o utils.o
	gcc -g -Wall -lm -lpthread pth_trapezoidal_rule.o utils.o -o pth_trapezoidal_rule

clean:
	rm -f pth_trapezoidal_rule pth_approx_pi
