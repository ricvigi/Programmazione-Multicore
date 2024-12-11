#include <stdio.h>
#include <cuda.h>

__global__ void hello()
{
    printf("Hello, World\n");
}

int main(int argc, char** argv)
{
    hello<<<1,10>>>();
    cudaDeviceSynchronize();
    return 0;
}
