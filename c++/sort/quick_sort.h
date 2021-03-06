/****************************************************************
    File Name    : quick_sort.h
    Author       : Jun Zhang
	Mail         : ewalker.zj@gmail.com 
	Created Time : 2017-12-17 10-09
*****************************************************************/
// http://www.csl.mtu.edu/cs2321/www/newLectures/11_Quick_Sort.html
// o(nlogn), o(nlogn), o(n*n) o(nlogn) unsteady

#ifndef _QUICK_SORT_H
#define _QUICK_SORT_H

#include<iostream>
#include<cstdlib>
#include<time.h>
#include "common.h"

using namespace std;

int partition(int arr[], int begin, int end){
    int pivot = arr[end];
    int i = begin;
    for(int j = begin; j <= end - 1; j++){
        if(arr[j] <= pivot){
            swap(arr[i ++], arr[j]);
        }
    }
    swap(arr[i], arr[end]);
    return i;
}

int partition(int arr[], int begin, int end){
    srand((unsigned)time(0));
    int idx = rand() % (end - begin + 1) + begin;
    //cout << idx << endl;
    int pivot = arr[idx];
    
    swap(arr[idx], arr[begin]);

    int i = begin + 1;
    int j = end;
    while(i <= j){
        while(i <= end && arr[i] <= pivot)
            i ++;
        while(j >= begin && arr[j] > pivot)
            j --;
        if(i < j){
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[begin], arr[j]);
    return j;
}

int partition_v2(int arr[], int begin, int end) {
    srand((unsigned)time(0));
    int rand_idx = rand() % (end - begin + 1) + begin;
    int povit = arr[rand_idx];
    swap(arr[begin], arr[rand_idx]);

    while(begin < end) {
        while(begin < end && arr[end] >= povit) { 
            end --;
        }
        arr[begin] = arr[end];
        while(begin < end && arr[begin] <= povit) { 
            begin ++;
        }
        arr[end] = arr[begin];
    }
    arr[begin] = povit;
    return begin;
}


void quick_sort_recursive(int arr[], int begin, int end){
    if(begin < end){
        int idx = partition(arr, begin, end);
        quick_sort_recursive(arr, begin, idx - 1);
        quick_sort_recursive(arr, idx + 1, end);
    }
}

void quick_sort(int* A, int start , int end)
{
    if(A == NULL || start >= end)
        return;

    int key = A[start];

    int left = start + 1;
    int right = end;

    while(left <= right)//脳垄脪芒碌脠潞脜
    {   
        while(A[left] <= key && left <= right)//脳垄脪芒碌脠潞脜
            left ++; 
        while(A[right] > key && left <= right)//脳垄脪芒碌脠潞脜
            right --; 

        if(left < right)
        {
            //printf("swap %d & %d\n", A[left], A[right]);
            swap(A[left], A[right]);
            left++;
            right--;
        }

    }
    //printlist(A, end - start + 1);
    //printf("left %d righ %d\n", left, right);

    swap(A[start], A[right]);

    quick_sort(A, start, left - 1);
    quick_sort(A, left + 1, end);
}

void quick_sort(int arr[], int len){
    quick_sort_recursive(arr, 0, len - 1);
}

#endif
