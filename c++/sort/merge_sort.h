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
    
}

void merge_sort_recursive(int arr[], int begin, int end){
    int mid = (left + right) / 2;
    if(begin < end){
        merge_sort_recursive(arr, begin, mid);
        merge_sort_recursive(arr, mid + 1, end);
        merge(arr, begin, mid, right);
    }
}

void merge_sort(int arr[], int len){
    merge_sort_recursive(arr, 0, len-1);
}

#endif
