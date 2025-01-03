#include <sys/time.h>
#include <cstdio>
#include "error_checks.h"

// Change this to 0 if CPU reference result is not needed
#define COMPUTE_CPU_REFERENCE 1
#define MAX_ITERATIONS 3000
#define GPU_VERSION 1 // TODO: Change this to 1 to enable the GPU version

// CPU kernel
void sweepCPU(double* phi, const double *phiPrev, const double *source, 
              double h2, int N)
{ 
    int i, j;
    int index, i1, i2, i3, i4;


    for (j = 1; j < N-1; j++) {
        for (i = 1; i < N-1; i++) {
            index = i + j*N; 
            i1 = (i-1) +   j   * N; // Previous element
            i2 = (i+1) +   j   * N; // Next element
            i3 =   i   + (j-1) * N; // Top element
            i4 =   i   + (j+1) * N; // Bottom element
            phi[index] = 0.25 * (phiPrev[i1] + phiPrev[i2] + 
                                 phiPrev[i3] + phiPrev[i4] - 
                                 h2 * source[index]);
        } 
    }
} 

// GPU kernel. ATTENTION: this might very easily not work
__global__
void sweepGPU(double *phi, const double *phiPrev, const double *source,
              double h2, int N)
{
    // TODO: Add here the GPU implementation
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;
    int index = i + j*N;
    int i1, i2, i3, i4;

    i1 = (i - 1) + j*N;
    i2 = (i + 1) + j*N;
    i3 = i + (j-1)*N;
    i4 = i + (j+1)*N;

    if (i > 0 && j > 0 && i < N - 1 && j < N - 1) {
        phi[index] = 0.25 * (phiPrev[i1] + phiPrev[i2] +
                             phiPrev[i3] + phiPrev[i4] -
                             h2 * source[index]);
    }
}

double compareArrays(const double *a, const double *b, int N)
{
    double error = 0.0;
    int i;
    for (i = 0; i < N*N; i++) {
        error += fabs(a[i] - b[i]);
    }
    return error/(N*N);
}


double diffCPU(const double *phi, const double *phiPrev, int N)
{
    int i;
    double sum = 0;
    double diffsum = 0;
    
    for (i = 0; i < N*N; i++) {
        diffsum += (phi[i] - phiPrev[i]) * (phi[i] - phiPrev[i]);
        sum += phi[i] * phi[i];
    }

    return sqrt(diffsum/sum);
}

/* ATTENTION: You have to implement sqrt(diffsum/sum) OUTSIDE this function*/
__global__
void diffGPU(const double* phi, const double* phiPrev, int N, double* sum, double* diffsum)
{
    // TODO: Add here GPU implementation
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;
    int index = i + j*N;
    if (0 <= index < N*N) {
        atomicAdd(diffsum, (phi[index] - phiPrev[index]) * (phi[index] - phiPrev[index]));
        atomicAdd(sum, (phi[i] * phi[i]));
    }
}

int main() 
{ 
    timeval t1, t2; // Structs for timing
    const int N = 512;
    double h = 1.0 / (N - 1);
    int iterations;
    const double tolerance = 5e-4; // Stopping condition
    int i, j, index;

    const int blocksize = 16;
  
    double *phi      = new double[N*N]; 
    double *phiPrev  = new double[N*N]; 
    double *source   = new double[N*N]; 
    double *phi_cuda = new double[N*N]; 

    double *phi_d, *phiPrev_d, *source_d; 
    // Size of the arrays in bytes
    const int size = N*N*sizeof(double); 
    double diff;
  
    // Source initialization
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {      
            double x, y;
            x = (i - N / 2) * h;
            y = (j - N / 2) * h;
            index = j + i * N;
            if (((x - 0.25) * (x - 0.25) + y * y) < 0.1 * 0.1)
                source[index] = 1e10*h*h;
            else if (((x + 0.25) * (x + 0.25) + y * y) < 0.1 * 0.1)
                source[index] = -1e10*h*h;
            else
                source[index] = 0.0;
        }            
    }

    CUDA_CHECK( cudaMalloc( (void**)&source_d, size) ); 
    CUDA_CHECK( cudaMemcpy(source_d, source, size, cudaMemcpyHostToDevice) ); 

    // Reset values to zero
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {      
            index = j + i * N;
            phi[index] = 0.0; 
            phiPrev[index] = 0.0; 
        }            
    }

    CUDA_CHECK( cudaMalloc( (void**)&phi_d, size) ); 
    CUDA_CHECK( cudaMalloc( (void**)&phiPrev_d, size) ); 
    CUDA_CHECK( cudaMemcpy(phi_d, phi, size, cudaMemcpyHostToDevice) );
    CUDA_CHECK( cudaMemcpy(phiPrev_d, phiPrev, size, cudaMemcpyHostToDevice) );

    // CPU version 
    if(COMPUTE_CPU_REFERENCE) { 
        gettimeofday(&t1, NULL);

        // Do sweeps untill difference is under the tolerance
        diff = tolerance * 2;
        iterations = 0;
        while (diff > tolerance && iterations < MAX_ITERATIONS) {
            sweepCPU(phiPrev, phi, source, h * h, N);
            sweepCPU(phi, phiPrev, source, h * h, N);
            
            iterations += 2;
            if (iterations % 100 == 0) {
                diff = diffCPU(phi, phiPrev, N);
                printf("%d %g\n", iterations, diff);
            }
        }
        gettimeofday(&t2, NULL);
        printf("CPU Jacobi: %g seconds, %d iterations\n", 
               t2.tv_sec - t1.tv_sec + 
               (t2.tv_usec - t1.tv_usec) / 1.0e6, iterations);
    }


#if GPU_VERSION
    // GPU version
    printf("GPU:\n");
    /* variables needed by diffGPU */
    double* diffsum_d;
    double* sum_d;


    CUDA_CHECK( cudaMalloc( (void**)&diffsum_d, sizeof(double)) );
    CUDA_CHECK( cudaMalloc( (void**)&sum_d, sizeof(double)) );
    dim3 dimBlock(blocksize, blocksize); 
    dim3 dimGrid((N + blocksize - 1) / blocksize, (N + blocksize - 1) / blocksize); 
    
    //do sweeps until diff under tolerance
    diff = tolerance * 2;
    iterations = 0;

    gettimeofday(&t1, NULL);

    while (diff > tolerance && iterations < MAX_ITERATIONS)
    {
        // See above how the CPU update kernel is called
        // and implement similar calling sequence for the GPU code

        //// Add routines here
        // TODO: Add GPU kernel calls here (see CPU version above)
        sweepGPU<<<dimGrid,dimBlock>>>(phiPrev_d, phi_d, source_d, h * h, N);
        CUDA_CHECK( cudaDeviceSynchronize() );
        sweepGPU<<<dimGrid,dimBlock>>>(phi_d, phiPrev_d, source_d, h * h, N);
        CUDA_CHECK( cudaDeviceSynchronize() );
        iterations += 2;
        if (iterations % 100 == 0)
        {
            // TODO: Add GPU kernel calls here (see CPU version above) 
            double  diffsum_h = 0.0;
            double sum_h = 0.0;
            CUDA_CHECK( cudaMemset(diffsum_d, 0.0, sizeof(double)) );
            CUDA_CHECK( cudaMemset(sum_d, 0.0, sizeof(double)) );
            diffGPU<<<dimGrid,dimBlock>>>(phi_d, phiPrev_d, N, sum_d, diffsum_d);
            CUDA_CHECK( cudaDeviceSynchronize() );
            CUDA_CHECK( cudaMemcpy(&diffsum_h, diffsum_d, sizeof(double), cudaMemcpyDeviceToHost) );
            CUDA_CHECK( cudaMemcpy(&sum_h, sum_d, sizeof(double), cudaMemcpyDeviceToHost) );
            diff = sqrt(diffsum_h / sum_h);
            printf("diff: %f, diffsum_d: %f, sum_d:%f\n", diff, diffsum_h, sum_h);
            fflush(stdout);
            CHECK_ERROR_MSG("Difference computation");
            printf("%d %g\n", iterations, diff);
        }
    }
    
    //// Add here the routine to copy back the results
    //TODO: Copy back the results
    CUDA_CHECK( cudaMemcpy(phi_cuda, phi_d, N*N, cudaMemcpyDeviceToHost) );

    gettimeofday(&t2, NULL);
    printf("GPU Jacobi: %g seconds, %d iterations\n", 
           t2.tv_sec - t1.tv_sec + 
           (t2.tv_usec - t1.tv_usec) / 1.0e6, iterations);

    //// Add here the clean up code for all allocated CUDA resources
    // TODO: Add here the clean up code
    CUDA_CHECK( cudaFree((void*)diffsum_d) );
    CUDA_CHECK( cudaFree((void*)sum_d) );
    CUDA_CHECK( cudaDeviceReset() );
#endif

    if (COMPUTE_CPU_REFERENCE)
    {
        printf("Average difference is %g\n", compareArrays(phi, phi_cuda, N));
    }
    
    delete[] phi; 
    delete[] phi_cuda;
    delete[] phiPrev; 
    delete[] source; 

    
    return EXIT_SUCCESS; 
} 
