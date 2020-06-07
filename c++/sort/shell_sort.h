/****************************************************************
    File Name    : shell_sort.h
    Author       : Jun Zhang
	Mail         : ewalker.zj@gmail.com 
	Created Time : 2017-12-17 10-07
*****************************************************************/
// 增量分组，每组插入排序，直至增量减至1
//o(n^1.3), o(n), o(n*n) o(1) unsteady

//https://www.geeksforgeeks.org/shellsort/

#ifndef _SHELL_SORT_H
#define _SHELL_SORT_H

#include <iostream>
#include "common.h"
using namespace std;


void shell_sort(int arr[], int len){
    for(int gap = len/2; gap > 0; gap /= 2){
        
        for(int i = gap; i < len; i++ ){
            int tmp = arr[i];

            int j;
            for(j = i; j >= gap && arr[j - gap] > tmp; j -=  gap)
                arr[j] = arr[j - gap];

            arr[j] = tmp;
        }
    }

}

void shell_sort_v2(int arr[], int len) {
    int gap = 1;
    while(gap < len) {
        gap = gap * 3 + 1;
    }

    while(gap > 0) {
        for(int i = gap; i < len; i ++) {
            int key = arr[i];
            int j = i - gap;
            while(j >= 0 && arr[j] > key) {
                arr[j + gap] = arr[j];
                j -= gap;
            }
            arr[j + gap] = key;
        }
        gap = (gap - 1) / 3;
    }
}

#endif

