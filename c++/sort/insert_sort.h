/****************************************************************
    File Name    : insert_sort.h
    Author       : Jun Zhang
	Mail         : zhangjun9@staff.weibo.com 
	Created Time : 2017-12-16 17-12
*****************************************************************/
// 将待元素插入前面已排序元素中
// o(n*n), o(n), o(n*n) o(1) steady 

#ifndef _INSERT_SORT_H
#define _INSERT_SORT_H

#include<iostream>
using namespace std;

void insert_sort(int arr[], int n){
    int i, j;
    int key;

    for(i = 1; i < n; i++){
        key = arr[i];
        j = i - 1;

        while(j >= 0 && arr[j] > key){
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;   // current
    }
}

#endif

