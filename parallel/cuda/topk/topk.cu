#include <iostream>
#include <cuda_runtime.h>
#include <algorithm>
#include <cuda.h>
#include <thrust/sort.h>
#include <thrust/functional.h>
#include <thrust/execution_policy.h>
#include <thrust/device_vector.h>

#include "timer.h"

struct Item {
    float x;
    int y;
};

template<typename T>
struct cmp {
    __host__ __device__ 
    bool operator()(const T& lsh, const T& rsh) {
        return lsh.x > rsh.x;
    }
};

template<typename T>
void topk_thrust_v1(thrust::device_vector<T>& input, int k, T* output){
    //thrust::sort(thrust::device, input.begin(), input.end(), thrust::greater<T>());
    thrust::sort(thrust::device, input.begin(), input.end(), cmp<T>());
    thrust::copy(input.begin(), input.begin() + k, output);
}

int main(){
    Timer timer;
    int k = 100;
    int num = 70000000;

    Timer tt;
    tt.Init();
    Item *output = (Item*)malloc(k * sizeof(Item));

    Item *input = (Item*)malloc(num * sizeof(Item));

    srand((unsigned)time(NULL));
    for(int i = 0; i < num; i ++){
        input[i].x = rand() / double(100000);
        input[i].y = i;
    }

    long long cost = timer.GetTimeElapsed();
    std::cout << "cost: " << cost << std::endl;

    thrust::device_vector<Item> data(input, input + num);

    timer.Init();
    topk_thrust_v1<Item>(data, k, output);
    //topk_thrust(input, num, k, output);

    //cpu calc
    //topk_cpu(input, num, k, output);

    for(int i = 0; i < k; i ++)
        std::cout << output[i].y << std::endl;

    long long cost = timer.GetTimeElapsed();
    std::cout << "cost: " << cost << std::endl;

    return 0;
}
