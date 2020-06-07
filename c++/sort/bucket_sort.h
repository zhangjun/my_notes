/*************************************************************************
   File Name    : bucket_sort.h
   Author       : Jun Zhang
   Email        : ewalker.zj@gmail.com
   Created Time : 2020-02-17 07:44
   Description  : 
*************************************************************************/

//https://www.geeksforgeeks.org/bucket-sort-2/

#pragma once
#include <vector>
#include <algorithm>


void bucket_sort(int arr[], int len, int bucket_size) {
    int min = arr[0], max = arr[0];
    for(int i = 0; i < len; i ++) {
        if(arr[i] < min)
            min = arr[i];
        if(arr[i] > max)
            max = arr[i];
    }

    int bucket_count = (max - min) / bucket_size + 1;
    //std::cout << "max: " << max << ", min: " << min << ", bucket_count: " << bucket_count << std::endl;
    std::vector<std::vector<int>> buckets(bucket_count, std::vector<int>()); 

    for(int i = 0; i < len; i ++) {
        int bucket_idx = (arr[i] - min) / bucket_size;
        buckets[bucket_idx].push_back(arr[i]);
    }

    int idx = 0;
    for(auto &it : buckets) {
        if(it.size() == 0) continue;

        sort(it.begin(), it.end());
        for(auto item: it) {
            arr[idx ++] = item;
            //std::cout << "idx: " << idx << ", item: " << item << std::endl;
        }
    }
}

void bucket_sort(int arr[], int len) {
    int bucket_size = 5;
    bucket_sort(arr, len, bucket_size);
}

