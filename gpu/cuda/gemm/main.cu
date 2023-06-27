#include "cuda_runtime.h"
#include "timer.h"
#include <iostream>
#include <time.h>

// https://siboehm.com/articles/22/CUDA-MMM

#define CEIL_DIV(x, y) (x + y - 1) / y
__global__ void empty() {}

void matmul(const float *A, const float *B, float *C, int M, int K, int N) {
  for (int i = 0; i < M; ++i) {
    for (int j = 0; j < N; ++j) {
      float sum = 0.0f;
      for (int k = 0; k < K; ++k) {
        sum += A[i * K + k] * B[k * N + j];
      }
      C[i * N + j] = sum;
    }
  }
}

__global__ void matmul_kernel(const float *A, const float *B, float *C, int M,
                              int K, int N) {
  int tx = blockIdx.x * blockDim.x + threadIdx.x; // M
  int ty = blockIdx.y * blockDim.y + threadIdx.y; // N

  if (tx < M && tx < N) {
    float sum = 0.0f;
    for (int k = 0; k < K; ++k) {
      sum += A[tx * K + k] * B[k * N + ty];
    }
    C[tx * N + ty] = sum;
  }
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

  int M = 256, K = 64, N = 128;
  float *A = new float[M * K];
  float *B = new float[K * N];
  float *C = new float[M * N];
  float *gpu_C = new float[M * N];

  float *d_A, *d_B, *d_C;
  cudaMalloc(&d_A, M * K * sizeof(float));
  cudaMalloc(&d_B, K * N * sizeof(float));
  cudaMalloc(&d_C, M * N * sizeof(float));

  // generate data
  for (int i = 0; i < M * K; ++i) {
    A[i] = rand() % 100 * 2.2;
  }

  for (int i = 0; i < K * N; ++i) {
    B[i] = rand() % 100 * 2.2;
  }

  CPUTimer cpu_timer;
  cpu_timer.Reset();
  matmul(A, B, C, M, K, N);
  float cpu_cost = cpu_timer.ElapsedTime();
  std::cout << "cpu time cost: " << cpu_cost << " ms." << std::endl;

  // warmup
  dim3 blockDim(32, 32, 1);
  dim3 gridDim(CEIL_DIV(M, 32), CEIL_DIV(N, 32), 1);

  GPUTimer gpu_timer;
  gpu_timer.Start(stream);
  cudaMemcpy(d_A, A, M * K * sizeof(float), cudaMemcpyHostToDevice);
  cudaMemcpy(d_B, B, K * N * sizeof(float), cudaMemcpyHostToDevice);
  matmul_kernel<<<gridDim, blockDim>>>(d_A, d_B, d_C, M, K, N);
  cudaMemcpy(gpu_C, d_C, M * N * sizeof(float), cudaMemcpyDeviceToHost);
  gpu_timer.Stop(stream);
  float gpu_cost = gpu_timer.ElapsedTime();
  std::cout << "gpu time cost: " << gpu_cost << " ms." << std::endl;

  bool has_diff = compare_diff(C, gpu_C, M * N);
  std::cout << "diff: " << has_diff << std::endl;

  // std::cout << "launch latency: " << elapse_time / (1.0f * repeat) << " ms."
  // << std::endl;
  return 0;
}