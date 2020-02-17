/****************************************************************
    File Name    : radix_sort.h
    Author       : Jun Zhang
	Mail         : ewalker.zj@gmail.com 
	Created Time : 2017-12-17 10-10
*****************************************************************/
// O((n+b) * logb(k)),    (n+b)

#ifndef _RADIX_SORT_H
#define _RADIX_SORT_H

#include<iostream>
using namespace std;

int GetMax(int arr[], int n) {
    int result = arr[0];
    for(int i = 1; i < n; i ++) {
        result = (arr[i] > result) ? arr[i] : result;
    }
    return result;
}

void count_sort(int arr[], int n, int exp) {
    int output[n];
    int count[10] = {0};
    
    // Store count of occurrences in count[] 
    for(int i = 0; i < n; i ++) {
        count[(arr[i] / exp) % 10] ++;
    }

    // Change count[i] so that count[i] now contains actual 
    //  position of this digit in output[] 
    for(int i = 1; i < 10; i ++) {
        count[i] += count[i - 1];
    }

    // Build the output array, decrease idx to keep order
    for(int i = n - 1; i > 0; i --) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];      // build output keep order
        count[(arr[i] / exp) % 10] --;     // occurrence decrease
    }

    for(ini i = 0; i < n; i ++) {
        arr[i] = output[i];
    }
}

void radix_sort(int arr[], int n) {
    // Find the maximum number to know number of digits
    int max = GetMax(arr, n);
    
    for(int exp = 1; max / exp > 0; exp *= 10) {
        count_sort(arr, n, exp);
    }
}

#endif
