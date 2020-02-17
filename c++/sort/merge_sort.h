/****************************************************************
    File Name    : merge_sort.h
    Author       : Jun Zhang
	Mail         : ewalker.zj@gmail.com 
	Created Time : 2017-12-17 10-10
*****************************************************************/

#ifndef _MERGE_SORT_H
#define _MERGE_SORT_H

#include<iostream>
using namespace std;

void merge(int arr[], int left, int mid, int right){
    int *buffer = new int[right - left + 1];
    int first_idx = left, second_idx = mid + 1;
    int idx = 0;
    while(first_idx <= mid && second_idx <= right) {
        if(arr[first_idx] <= arr[second_idx])
            buffer[idx ++] = arr[first_idx ++];
        else 
            buffer[idx ++] = arr[second_idx ++];
    }

    while(first_idx <= mid) 
        buffer[idx ++] = arr[first_idx ++];
    while(second_idx <= right) 
        buffer[idx ++] = arr[second_idx ++];

    for(int i = 0, j = left; j <= right; i ++, j ++) {
        arr[j] = buffer[i];
    }

    delete []buffer;
}

void merge_sort_recursive(int arr[], int begin, int end){
    int mid = (begin + end) / 2;
    if(begin < end){
        merge_sort_recursive(arr, begin, mid);
        merge_sort_recursive(arr, mid + 1, end);
        merge(arr, begin, mid, end);
    }
}

void merge_sort(int arr[], int len){
    merge_sort_recursive(arr, 0, len-1);
}

#endif
