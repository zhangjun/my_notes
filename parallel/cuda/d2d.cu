#include <stdio.h>

#include "cuda_runtime.h"
#include "cuda_util.h"

int main() {
    const int count = 1024 * 1024;
    int *first = NULL, *second = NULL;

    int device_count;
    cudaGetDeviceCount(&device_count);
    printf("device count: %d\n", device_count);


    {
        CUDACHECK(cudaSetDevice(0));
        CUDACHECK(cudaMalloc((void**)&first, sizeof(int) * count));
        CUDACHECK(cudaMemset(first, 0, sizeof(int) * count)); 
    }

    {
        CUDACHECK(cudaSetDevice(1));
        CUDACHECK(cudaMalloc((void**)&second, sizeof(int) * count));
        CUDACHECK(cudaMemset(second, 1, sizeof(int) * count)); 
    }

    cudaMemcpy(first, second, sizeof(int) * count, cudaMemcpyDeviceToDevice);

    {
        CUDACHECK(cudaSetDevice(1));
        int *data = (int*)malloc(sizeof(int) * count);
        memset(data, 0, sizeof(int) * count); 
        CUDACHECK(cudaMemcpy(data, first, sizeof(int) * count, cudaMemcpyDeviceToHost)); 
        printf("data: %d\n", data[0]);

        free(data);
    }

    cudaFree(first);
    cudaFree(second);

    return 0;

}
