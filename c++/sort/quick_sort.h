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

using namespace std;

void swap(int *first, int *second){
    int tmp = *first;
    *first = *second;
    *second = *first;
}

int partition(int arr[], int begin, int end){
    int pivot = arr[end];
    int i = (begin - 1);
    int j;
    for(j = begin; j <= end - 1; j++){
        if(arr[j] <= pivot){
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i+1], arr[end]);
    return i + 1;
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

void quick_sort_recursive(int arr[], int begin, int end){
    if(begin < end){
        int idx = partition(arr, begin, end);
        quick_sort_recursive(arr, begin, idx - 1);
        quick_sort_recursive(arr, idx + 1, end);
    }
}

void quick_sort(int arr[], int len){
    quick_sort_recursive(arr, 0, len - 1);
}

#endif
