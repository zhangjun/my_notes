#include "cuda_runtime.h"
#include <iostream>

__global__ void empty() {}

int main() {
  int device_id = 0;
  cudaSetDevice(device_id);

  cudaStream_t stream;
  cudaStreamCreate(&stream);

  cudaEvent_t start, end;
  cudaEventCreate(&start);
  cudaEventCreate(&end);

  int repeat = 1000;

  cudaEventRecord(start, stream);
  for (int i = 0; i < repeat; ++i) {
    empty<<<1, 1>>>();
  }
  cudaEventRecord(end, stream);
  cudaEventSynchronize(end);
  float elapse_time = 0.0f;
  cudaEventElapsedTime(&elapse_time, start, end);
  std::cout << "launch latency: " << elapse_time / (1.0f * repeat) << " ms."
            << std::endl;
  return 0;
}