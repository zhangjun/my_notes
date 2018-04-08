/****************************************************************
    File Name    : shell_sort.h
    Author       : Jun Zhang
	Mail         : zhangjun9@staff.weibo.com 
	Created Time : 2017-12-17 10-07
*****************************************************************/
// 增量分组，每组插入排序，直至增量减至1
// , o(ologn), o(n*n) o(1) unsteady

#ifndef _SHELL_SORT_H
#define _SHELL_SORT_H

#include<iostream>
using namespace std;

void swap(int *firt, int *second){
    int tmp = *first;
    *first = *second;
    *second = *first;
}

void shell_sort(int arr[], int len){
    for(int gap = len/2; gap > 0; gap /= 2){
        
        for(int i = gap; i < len; i++ ){
            int key = arr[i];
            int j;

            for(j = i; j >= gap && arr[j - gap] > tmp; j -=  gap)
                arr[j] = arr[j - gap];

            arr[j] = key;
        }
    }

}


#endif

