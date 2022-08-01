#include <iostream>
#include <cuda_runtime.h>
#include <chrono>
#include <cstdio>
#include <cuda_fp16.h>   // CUDA_VERSION >= 7050

#define CUDA_ARCH_FP16_SUPPORTED(CUDA_ARCH) (CUDA_ARCH >= 600)

template <typename T>
__device__ T math_exp(T a);

template <>
__device__ half math_exp<half>(half a) {
#if CUDA_ARCH_FP16_SUPPORTED(__CUDA_ARCH__)
  return hexp(a);
#endif
}

template <>
__device__ float math_exp<float>(float a) {
  return expf(a);
}

template <typename T>
__global__ void swish_kernel(int num, const T *input, T *output, T beta) {
  int index = blockIdx.x * blockDim.x + threadIdx.x;
  if (index < num) {
#if __CUDA_ARCH__ >= 350
    output[index] =
        __ldg(input + index) /
        (static_cast<T>(1.0) + math_exp<T>(-beta * __ldg(input + index)));
#else
    output[index] = input[index] /
                    (static_cast<T>(1.0) + math_exp<T>(-beta * input[index]));
#endif
  }
}

template <>
__global__ void swish_kernel<half>(int num,
                                   const half *input,
                                   half *output,
                                   half beta) {
  int index = blockIdx.x * blockDim.x + threadIdx.x;
  // half2* input2 = (half2*)input;
  // half2* output2 = (half2*)output;
  if (index < num) {
// #if CUDA_ARCH_FP16_SUPPORTED(__CUDA_ARCH__)
    output[index] =
        __ldg(input + index) /
        (static_cast<half>(1.0) + math_exp<half>(-beta * __ldg(input + index)));
    // output[index] = __hdiv(__ldg(input + index), (__hadd(static_cast<half>(1.0), hexp(__hneg(beta * (__ldg(input + index)))))));

    // output2[index] = __h2div(__ldg(input2 + index), (__hadd2(__halves2half2(1.0, 1.0), hexp2(__hneg2(__hmul2(__halves2half2(beta,beta), (__ldg(input2 + index))))))));
    // output2[index] = __h2div(input2[index], (__hadd2(__halves2half2(1.0, 1.0), hexp2(__hneg2(__hmul2(__halves2half2(beta, beta), (input2[index])))))));
    // half2 val = __halves2half2(1.0, 1.0);
    printf("hehe");

// #endif
  }
}

int main() {
    using dtype = half;
    int num = 1 * 64 * 112 * 112;
    int size = num * sizeof(dtype);
    half beta = 1.0;
    int threads = 1024;
    int blocks = (num + threads - 1) / threads;
    
    dtype* h_input = new dtype[num];
    dtype* h_output = new dtype[num];
    h_input[0] = 5;
    
    int device_id = 2;
    cudaSetDevice(device_id);
    cudaStream_t stream;
    cudaStreamCreate(&stream);
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    dtype *input, *output;
    cudaMalloc(&input, size);
    cudaMalloc(&output, size);

    // warmup
    for (int i = 0; i < 5; i ++)
    {
        cudaMemcpyAsync(input, h_input, size, cudaMemcpyHostToDevice, stream);
        swish_kernel<<<blocks, threads, 0, stream>>>(num, input, output, beta);
        cudaMemcpyAsync(h_output, output, size, cudaMemcpyDeviceToHost, stream);
    }
    cudaStreamSynchronize(stream);

    // run
    int repeats = 1; 
    float cost = 0.0f;

    for (int i = 0; i < repeats; i ++) {
        cudaMemcpyAsync(input, h_input, size, cudaMemcpyHostToDevice, stream);
        cudaEventRecord(start, stream);

        swish_kernel<<<blocks, threads, 0, stream>>>(num, input, output, beta);

        cudaEventRecord(stop, stream);
        cudaEventSynchronize(stop);
        float elapse_time = 0.0f;
        cudaEventElapsedTime(&elapse_time, start, stop);
        cost += elapse_time;
        // std::cout << elapse_time << std::endl;
        cudaMemcpyAsync(h_output, output, size, cudaMemcpyDeviceToHost, stream);
    }
    cudaStreamSynchronize(stream);
    
    for(int i = 0; i < 6; i ++) {
      std::cout << __half2float(h_output[i]) << std::endl;
    }
    float avg_cost = cost / (1.0f * repeats);
    std::cout << "avg cost: " << avg_cost << std::endl;
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    cudaFree(input);
    cudaFree(output);
    
    return 0;
}