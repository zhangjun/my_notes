#include <iostream>
#include <cuda_runtime.h>
#include <stdio.h>

#define THREADS 512 // 2^9
#define BLOCKS 32768 // 2^15
#define NUM_VALS THREADS*BLOCKS

__device__ void swap(unsigned int a, unsigned int b, float *data){
	float temp = data[a];
	data[a]=data[b];
	data[b]=temp;
}

__global__ void bitonic_sort_step(float *dev_values, int j, int k)
{
  unsigned int i, ixj; /* Sorting partners: i and ixj */
  i = threadIdx.x + blockDim.x * blockIdx.x;
  extern __shared__ float buf[];

  ixj = i^j;

  /* The threads with the lowest ids sort the array. */
  if ((ixj)>i) {
    if ((i&k)==0) {
      /* Sort ascending */
      if (dev_values[i]>dev_values[ixj]) {
        /* exchange(i,ixj); */
        swap(i, ixj, dev_values);
      }
    }
    if ((i&k)!=0) {
      /* Sort descending */
      if (dev_values[i]<dev_values[ixj]) {
        /* exchange(i,ixj); */
        swap(i, ixj, dev_values);
      }
    }
  }
}

void bitonic_sort(float *values, int count)
{
  float *dev_values;
  size_t size = count * sizeof(float);

  cudaMalloc((void**) &dev_values, size);
  cudaMemcpy(dev_values, values, size, cudaMemcpyHostToDevice);

  dim3 blocks(BLOCKS,1);    /* Number of blocks   */
  dim3 threads(THREADS,1);  /* Number of threads  */

  int j, k;
  /* Major step */
  for (k = 2; k <= count; k <<= 1) {
    /* Minor step */
    for (j=k>>1; j>0; j=j>>1) {
      bitonic_sort_step<<<blocks, threads>>>(dev_values, j, k);
    }
  }
  cudaMemcpy(values, dev_values, size, cudaMemcpyDeviceToHost);
  cudaFree(dev_values);
}

__global__ void D_bitonic_sort_K(float *data_to_sort)
{
    extern __shared__ float buf[];
    uint desc; // bit position that determines sort order ascending/descending
	uint stride; // distance to partner index
	int thread = threadIdx.x;

	const int idx = blockIdx.x * blockDim.x + threadIdx.x;

    buf[thread] = data_to_sort[thread];
    __syncthreads();

	for (desc=2; desc<=blockDim.x; desc*=2) {
		for (stride = desc>>1; stride>0; stride=stride>>1) {
			int partner = idx ^ stride;
			if (partner > idx) {
				if ( (thread & desc) == 0 && data_to_sort[idx] > data_to_sort[partner] ) {
					swap(idx,partner,data_to_sort);
				} else if ( (thread & desc) != 0 && data_to_sort[idx]< data_to_sort[partner] ) {
					swap(idx,partner,data_to_sort);
				}
            }
            __syncthreads();
		}
	}
}

int main() {
    float* data = (float*)malloc(NUM_VALS * sizeof(float));
    bitonic_sort(data, NUM_VALS);
}