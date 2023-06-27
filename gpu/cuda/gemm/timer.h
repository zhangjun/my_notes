#pragma once
#include "cuda_runtime.h"
#include <chrono>

class GPUTimer {
public:
  GPUTimer() {
    cudaEventCreate(&start_);
    cudaEventCreate(&stop_);
  }

  ~GPUTimer() {
    cudaEventDestroy(start_);
    cudaEventDestroy(stop_);
  }

  void Start(cudaStream_t stream) { cudaEventRecord(start_, stream); }

  void Stop(cudaStream_t stream) { cudaEventRecord(stop_, stream); }

  float ElapsedTime() {
    float milliseconds = 0;

    cudaEventSynchronize(stop_);
    cudaEventElapsedTime(&milliseconds, start_, stop_);
    return milliseconds;
  }

private:
  cudaEvent_t start_;
  cudaEvent_t stop_;
};

class CPUTimer {
public:
  using high_resolution_clock = std::chrono::high_resolution_clock;
  CPUTimer() {}
  ~CPUTimer() {}
  void Start() {}

  void Reset() { begin_ = high_resolution_clock::now(); }

  float ElapsedTime() {
    auto end = high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - begin_);
    return duration.count() / 1000.0;
  }

private:
  high_resolution_clock::time_point begin_;
};