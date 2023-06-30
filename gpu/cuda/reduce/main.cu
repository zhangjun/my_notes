#include "cuda_runtime.h"
#include "timer.h"
#include <iostream>
#include <time.h>

// https://github.com/BBuf/how-to-optim-algorithm-in-cuda/tree/master/reduce

#define CEIL_DIV(x, y) (x + y - 1) / y
__global__ void empty() {}


#include <cuda.h>
#include <cuda_runtime.h>
#include <time.h>

#define N 32*1024*1024
#define BLOCK_SIZE 256

__global__ void reduce_v0(float *g_idata,float *g_odata){
    __shared__ float sdata[BLOCK_SIZE];

    // each thread loads one element from global to shared mem
    unsigned int tid = threadIdx.x;
    unsigned int i = blockIdx.x*blockDim.x + threadIdx.x;
    sdata[tid] = g_idata[i];
    __syncthreads();

    // do reduction in shared mem
    for(unsigned int s=1; s < blockDim.x; s *= 2) {
        if (tid % (2*s) == 0) {
            sdata[tid] += sdata[tid + s];
        }
        __syncthreads();
    }

    // write result for this block to global mem
    if (tid == 0) g_odata[blockIdx.x] = sdata[0];
}

__global__ void reduce_v1(float *g_idata,float *g_odata){
    __shared__ float sdata[BLOCK_SIZE];

    // each thread loads one element from global to shared mem
    unsigned int tid = threadIdx.x;
    unsigned int i = blockIdx.x*blockDim.x + threadIdx.x;
    sdata[tid] = g_idata[i];
    __syncthreads();

    // do reduction in shared mem
    for(unsigned int s=1; s < blockDim.x; s *= 2) {
        // if (tid % (2*s) == 0) {
        //     sdata[tid] += sdata[tid + s];
        // }
        int index = 2 * s * tid;
        if (index < blockDim.x) {
            sdata[tid] += sdata[tid + s];
        }
        __syncthreads();
    }

    // write result for this block to global mem
    if (tid == 0) g_odata[blockIdx.x] = sdata[0];
}

__global__ void reduce_v2(float *g_idata,float *g_odata){
    __shared__ float sdata[BLOCK_SIZE];

    // each thread loads one element from global to shared mem
    unsigned int tid = threadIdx.x;
    unsigned int i = blockIdx.x*blockDim.x + threadIdx.x;
    sdata[tid] = g_idata[i];
    __syncthreads();

    // do reduction in shared mem
    for(unsigned int s=blockDim.x/2; s>0; s >>= 1) {
        if (tid < s){
            sdata[tid] += sdata[tid + s];
        }
        __syncthreads();
    }

    // write result for this block to global mem
    if (tid == 0) g_odata[blockIdx.x] = sdata[0];
}

bool compare_diff(const float *A, const float *B, int n) {
  for (int i = 0; i < n; ++i) {
    if (std::abs(A[i] - B[i]) > std::numeric_limits<float>::epsilon()) {
      return true;
    }
  }
  return false;
}

int main() {
    srand(time(0));
    int device_id = 0;
    cudaSetDevice(device_id);
    cudaStream_t stream;
    cudaStreamCreate(&stream);

    float *input_host = (float*)malloc(N*sizeof(float));
    float *input_device;
    cudaMalloc((void **)&input_device, N*sizeof(float));
    for (int i = 0; i < N; i++) input_host[i] = 2.0;

    int32_t block_num = (N + BLOCK_SIZE - 1) / BLOCK_SIZE;
    float *output_host = (float*)malloc((N / BLOCK_SIZE) * sizeof(float));
    float *output_device;
    cudaMalloc((void **)&output_device, (N / BLOCK_SIZE) * sizeof(float));

    GPUTimer gpu_timer;
    gpu_timer.Start(stream);    
    cudaMemcpy(input_device, input_host, N*sizeof(float), cudaMemcpyHostToDevice);

    gpu_timer.Start(stream);
    dim3 grid(N / BLOCK_SIZE, 1);
    dim3 block(BLOCK_SIZE, 1);
    reduce_v1<<<grid, block>>>(input_device, output_device);
    gpu_timer.Stop(stream);
    float compute_cost = gpu_timer.ElapsedTime();
    std::cout << "gpu compute time cost: " << compute_cost << " ms." << std::endl;
    cudaMemcpy(output_device, output_host, block_num * sizeof(float), cudaMemcpyDeviceToHost);
    gpu_timer.Stop(stream);
    float gpu_cost = gpu_timer.ElapsedTime();
    std::cout << "gpu time cost: " << gpu_cost << " ms." << std::endl;

    return 0;
}