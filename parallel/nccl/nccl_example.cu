#include <stdio.h>
#include "cuda_runtime.h"
#include "nccl.h"

#define CUDACHECK(cmd) do {                         \
  cudaError_t e = cmd;                              \
  if( e != cudaSuccess ) {                          \
    printf("Failed: Cuda error %s:%d '%s'\n",             \
        __FILE__,__LINE__,cudaGetErrorString(e));   \
    exit(EXIT_FAILURE);                             \
  }                                                 \
} while(0)


#define NCCLCHECK(cmd) do {                         \
  ncclResult_t r = cmd;                             \
  if (r!= ncclSuccess) {                            \
    printf("Failed, NCCL error %s:%d '%s'\n",             \
        __FILE__,__LINE__,ncclGetErrorString(r));   \
    exit(EXIT_FAILURE);                             \
  }                                                 \
} while(0)


void all_reduce()
{
  printf("\n#####AllReduce Begin#### \n");

  ncclComm_t comms[2];


  //managing 4 devices
  int nDev = 2;
  int size = 1024;
  int devs[2] = { 0, 1};


  //allocating and initializing device buffers
  int** sendbuff = (int**)malloc(nDev * sizeof(int*));
  int** recvbuff = (int**)malloc(nDev * sizeof(int*));
  cudaStream_t* s = (cudaStream_t*)malloc(sizeof(cudaStream_t)*nDev);


  for (int i = 0; i < nDev; ++i) {
    CUDACHECK(cudaSetDevice(i));
    CUDACHECK(cudaMalloc(sendbuff + i, size * sizeof(int)));
    CUDACHECK(cudaMalloc(recvbuff + i, size * sizeof(int)));
    CUDACHECK(cudaMemset(sendbuff[i], 1, size * sizeof(int)));
    CUDACHECK(cudaMemset(recvbuff[i], 0, size * sizeof(int)));
    CUDACHECK(cudaStreamCreate(s+i));
  }


  //initializing NCCL
  NCCLCHECK(ncclCommInitAll(comms, nDev, devs));


   //calling NCCL communication API. Group API is required when using
   //multiple devices per thread
  NCCLCHECK(ncclGroupStart());
  for (int i = 0; i < nDev; ++i)
    NCCLCHECK(ncclAllReduce((const void*)sendbuff[i], (void*)recvbuff[i], size, ncclInt, ncclSum,
        comms[i], s[i]));
  NCCLCHECK(ncclGroupEnd());



  //synchronizing on CUDA streams to wait for completion of NCCL operation
  for (int i = 0; i < nDev; ++i) {
    CUDACHECK(cudaSetDevice(i));
    CUDACHECK(cudaStreamSynchronize(s[i]));
  }


 {
     int idx = 0;
     int* result = (int*)malloc(size * sizeof(int));
     CUDACHECK(cudaSetDevice(idx));
     CUDACHECK(cudaMemcpy((void*)result, (void*)recvbuff[idx], size * sizeof(int), cudaMemcpyDeviceToHost));
     int* p = (int*)result;
     printf("check result: %d,%d\n", p[0], p[1]);
     free(result);
 }

  //free device buffers
  for (int i = 0; i < nDev; ++i) {
    CUDACHECK(cudaSetDevice(i));
    CUDACHECK(cudaFree(sendbuff[i]));
    CUDACHECK(cudaFree(recvbuff[i]));
  }


  //finalizing NCCL
  for(int i = 0; i < nDev; ++i)
      ncclCommDestroy(comms[i]);


  printf("AllReduce Success \n");
}

void broadcast() {
  printf("\n####Broadcast Begin#### \n");

  ncclComm_t comms[2];

  int nDev = 2;
  int devs[2] = {0, 1};
  int size = 32 * 1024 * 1024;


  int** buf = (int**)malloc(sizeof(int*) * nDev);
  cudaStream_t* s = (cudaStream_t*)malloc(sizeof(cudaStream_t) * nDev);

  for(int i = 0; i < nDev; i ++) {
      cudaSetDevice(i);
      cudaMalloc(buf + i, size * sizeof(int));
      cudaMemset(buf[i], i, size * sizeof(int));
      cudaStreamCreate(s + i);
  }

  NCCLCHECK(ncclCommInitAll(comms, nDev, devs));

  ncclGroupStart();
  for(int i = 0; i < nDev; i ++) {
      // sendbuf only used in root rank
      ncclBroadcast((const void*)buf[i], (void*)buf[i], size, ncclFloat, 1, comms[i], s[i]);
  }
  ncclGroupEnd();

  for(int i = 0; i < nDev; i ++) {
      cudaSetDevice(i);
      cudaStreamSynchronize(s[i]);
  }

  {
     int idx = 0;
     int* result = (int*)malloc(size * sizeof(int));
     CUDACHECK(cudaSetDevice(idx));
     CUDACHECK(cudaMemcpy((void*)result, (void*)buf[idx], size * sizeof(int), cudaMemcpyDeviceToHost));
     int* p = (int*)result;
     printf("check result: %d,%d\n", p[0], p[1]);
     free(result);
  }

  for(int i = 0; i < nDev; i ++) {
      cudaSetDevice(i);
      cudaFree(buf[i]);
  }

  for(int i = 0; i < nDev; i ++) {
      ncclCommDestroy(comms[i]);
  }

  printf("Broadcast Success \n");
}

void reduce() {
  printf("\n####Reduce Begin#### \n");

  ncclComm_t comms[2];

  int nDev = 2;
  int devs[2] = {0, 1};
  int size = 32 * 1024 * 1024;


  int** buf = (int**)malloc(sizeof(int*) * nDev);
  cudaStream_t* s = (cudaStream_t*)malloc(sizeof(cudaStream_t) * nDev);

  for(int i = 0; i < nDev; i ++) {
      cudaSetDevice(i);
      cudaMalloc(buf + i, size * sizeof(int));
      cudaMemset(buf[i], 1, size * sizeof(int));
      cudaStreamCreate(s + i);
  }

  NCCLCHECK(ncclCommInitAll(comms, nDev, devs));

  ncclGroupStart();
  for(int i = 0; i < nDev; i ++) {
      // recvbuf only used in root rank
      ncclReduce((const void*)buf[i], (void*)buf[i], size, ncclInt, ncclSum, 0, comms[i], s[i]);
  }
  ncclGroupEnd();

  for(int i = 0; i < nDev; i ++) {
      cudaSetDevice(i);
      cudaStreamSynchronize(s[i]);
  }

  {
     int idx = 0;
     int* result = (int*)malloc(size * sizeof(int));
     CUDACHECK(cudaSetDevice(idx));
     CUDACHECK(cudaMemcpy((void*)result, (void*)buf[idx], size * sizeof(int), cudaMemcpyDeviceToHost));
     int* p = (int*)result;
     printf("check result: %d,%d\n", p[0], p[1]);
     free(result);
  }

  for(int i = 0; i < nDev; i ++) {
      cudaSetDevice(i);
      cudaFree(buf[i]);
  }

  for(int i = 0; i < nDev; i ++) {
      ncclCommDestroy(comms[i]);
  }

  printf("Broadcast Success \n");
}


// all_gather = reduce_scatter + all_reduce
void all_gather() {
  printf("\n####Allgather Begin#### \n");

  ncclComm_t comms[2];

  int nDev = 2;
  int devs[2] = {0, 1};
  int size = 32 * 1024 * 1024;


  int** buf = (int**)malloc(sizeof(int*) * nDev);
  cudaStream_t* s = (cudaStream_t*)malloc(sizeof(cudaStream_t) * nDev);

  for(int i = 0; i < nDev; i ++) {
      cudaSetDevice(i);
      cudaMalloc(buf + i, nDev * size * sizeof(int));
      cudaMemset(buf[i], i, nDev * size * sizeof(int));
      cudaStreamCreate(s + i);
  }

  NCCLCHECK(ncclCommInitAll(comms, nDev, devs));

  ncclGroupStart();
  for(int i = 0; i < nDev; i ++) {
      // recvbuf only used in root rank
      ncclAllGather((const void*)buf[i], (void*)buf[i], size, ncclInt, comms[i], s[i]);
  }
  ncclGroupEnd();

  for(int i = 0; i < nDev; i ++) {
      cudaSetDevice(i);
      cudaStreamSynchronize(s[i]);
  }

  {
     int idx = 0;
     int* result = (int*)malloc(size * sizeof(int) * nDev);
     CUDACHECK(cudaSetDevice(idx));
     CUDACHECK(cudaMemcpy((void*)result, (void*)buf[idx], nDev * size * sizeof(int), cudaMemcpyDeviceToHost));
     int* p = (int*)result;
     printf("check result: %d,%d, %d,%d\n", p[0], p[1], p[size], p[size + 1]);
     free(result);
  }

  for(int i = 0; i < nDev; i ++) {
      cudaSetDevice(i);
      cudaFree(buf[i]);
  }

  for(int i = 0; i < nDev; i ++) {
      ncclCommDestroy(comms[i]);
  }

  printf("Allgather Success \n");
}


void reduce_scatter() {
  printf("\n####ReduceScatter Begin#### \n");

  ncclComm_t comms[2];

  int nDev = 2;
  int devs[2] = {0, 1};
  int size = 32 * 1024 * 1024;


  int** buf = (int**)malloc(sizeof(int*) * nDev);
  cudaStream_t* s = (cudaStream_t*)malloc(sizeof(cudaStream_t) * nDev);

  for(int i = 0; i < nDev; i ++) {
      cudaSetDevice(i);
      cudaMalloc(buf + i, nDev * size * sizeof(int));
      cudaMemset(buf[i], i, nDev * size * sizeof(int));
      cudaStreamCreate(s + i);
  }

  NCCLCHECK(ncclCommInitAll(comms, nDev, devs));

  ncclGroupStart();
  for(int i = 0; i < nDev; i ++) {
      // recvbuf only used in root rank
      ncclReduceScatter((const void*)buf[i], (void*)buf[i], size, ncclInt, ncclSum, comms[i], s[i]);
  }
  ncclGroupEnd();

  for(int i = 0; i < nDev; i ++) {
      cudaSetDevice(i);
      cudaStreamSynchronize(s[i]);
  }

  {
     int idx = 0;
     int* result = (int*)malloc(size * nDev * sizeof(int));
     CUDACHECK(cudaSetDevice(idx));
     CUDACHECK(cudaMemcpy((void*)result, (void*)buf[idx], nDev * size * sizeof(int), cudaMemcpyDeviceToHost));
     int* p = (int*)result;
     printf("check result: %d,%d %d,%d\n", p[0], p[1], p[size], p[size + 1]);
     free(result);
  }

  for(int i = 0; i < nDev; i ++) {
      cudaSetDevice(i);
      cudaFree(buf[i]);
  }

  for(int i = 0; i < nDev; i ++) {
      ncclCommDestroy(comms[i]);
  }

  printf("ReduceScatter Success \n");
}



int main(int argc, char* argv[]) {

  all_reduce();  

  broadcast();

  reduce();

  all_gather();

  reduce_scatter();

  return 0;
}
