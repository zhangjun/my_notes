/****************************************************************
    File Name    : bubble_sort.h
    Author       : Jun Zhang
	Mail         : ewalker.zj@gmail.com 
	Created Time : 2017-12-16 18-30
*****************************************************************/
// 交换相邻元素，每一趟将最大或者最小元素浮到顶端
// o(n*n), o(n), o(n*n) o(1) steady
//
#ifndef _BUBBLE_SORT_H
#define _BUBBLE_SORT_H

#include<iostream>
using namespace std;

void swap(int *first, int *second){
    int tmp = *first;
    *first = *second;
    *second = *first;
}

void bubble_sort(int arr[], int len){
    int i, j;
    bool sorted = true;

    for(i = 0; i < len - 1; i ++){

        for(j = 0; j < len - i - 1; j ++){
            if(arr[j + 1] > arr[j]){
                swap(arr[j], arr[j + 1]);
                sorted = false;
            }
        }
        if(sorted) break;
    }
}

#endif
