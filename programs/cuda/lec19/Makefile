CC          = nvcc
LIBS        =
# CFLAGS      = -arch=compute_80 -DBLUR_SIZE=1 -DBLOCK_SIZE=16
CFLAGS	    = -arch=sm_75 -DBLUR_SIZE=1 -DBLOCK_SIZE=16

RM=rm -f

#EXES=vector_add test_cuda image_blur image_blur_shared  jacobi jacobi_solution #image_blur_shared_all
EXES=jacobi
all: $(EXES)

%: %.cu 
	$(CC) $(CFLAGS) -o $@ $@.cu $(LIBS)

clean:
	$(RM) $(EXES) 
