#include <stdio.h>
#include <cuda.h>
#define MAX 100
#define CUDA_CHECK_RETURN(value) do { \
    cudaError_t _m_cudaStat = value; \
    if (_m_cudaStat != cudaSuccess) { \
        fprintf(stderr, "Error %s at line %d in file %s\n",\
        cudaGetErrorString(_m_cudaStat), __LINE__, __FILE__); \
        exit(1); \
    } \
} while(0)

void vecAdd (float* A, float* B, float* C, int n);
__global__ void vecAddKernel(float* A, float* B, float* C, int n);
float* create_random_vector(int n, int m);

int main(int argc, char** argv)
{
    int n = atoi(argv[1]);
    float* A = create_random_vector(n, MAX);
    float* B = create_random_vector(n, MAX);
    float* C = create_random_vector(n, MAX);

    vecAdd(A, B, C, n);
    for (int i = 0; i < n; i++) {
        printf("A[%d]%f + B[%d]%f = C[%d]%f", i, A[i], i, B[i], i, C[i]);
    }

    return EXIT_SUCCESS;
}

__global__ void vecAddKernel(float* A, float* B, float* C, int n)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    if (i < n) C[i] = A[i] + B[i];
}

void vecAdd (float* A, float* B, float* C, int n)
{
    int size = n * sizeof(float);
    float* d_A, *d_B, *d_C;

    CUDA_CHECK_RETURN(cudaMalloc((void**) &d_A, size));
    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
    CUDA_CHECK_RETURN(cudaMalloc((void**) &d_B, size));
    cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);
    CUDA_CHECK_RETURN(cudaMalloc((void**) &d_C, size));

    vecAddKernel<<<((n + 255) / 256),256>>>(d_A, d_B, d_C, n);
    cudaDeviceSynchronize();

    CUDA_CHECK_RETURN(cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost));
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
}

float* create_random_vector(int n, int m) {
    float* vec = (float*) malloc(n * sizeof(float));
    for(int i = 0; i < n; i++)
    {
        vec[i] = (float) (rand() % m);
    }
    return vec;
}
