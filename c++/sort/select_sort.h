/****************************************************************
    File Name    : select_sort.h
    Author       : Jun Zhang
	Mail         : ewalker.zj@gmail.com 
	Created Time : 2017-12-16 17-42
*****************************************************************/
// 每趟从待排序元素中选择最小的元素作为首元素
// o(n*n), o(n*n), o(n*n) o(1) unsteady

#ifndef _SIMPLE_SORT_H
#define _SIMPLE_SORT_H

#include<iostream>
#include "common.h"
using namespace std;

void select_sort(int arr[], int len){
    int i, j;
    int min;

    for(i = 0; i < len - 1; i ++){
       
        min = i;
        for(j = i + 1; j < len; j ++){
            if(arr[j] < arr[min])
                min = j;
        }
        swap(arr[min], arr[i]);

    }
}

#endif
