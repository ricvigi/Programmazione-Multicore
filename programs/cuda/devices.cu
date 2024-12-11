#include <stdio.h>
#include <cuda.h>

void countDevices()
{
    int deviceCount = 0;
    cudaGetDeviceCount(&deviceCount);
    if (deviceCount == 0) printf("No CUDA compatible GPU esists.\n");
    else
    {
        cudaDeviceProp pr;
        for (int i = 0 ; i < deviceCount; i++)
        {
            cudaGetDeviceProperties(&pr, i);
            printf("Dev #%i is %s\n", i, pr.name);
        }
    }
}

int main(int argc, char** argv) {
    countDevices();
    return 0;
}
