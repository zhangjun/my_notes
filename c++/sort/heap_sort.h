/****************************************************************
    File Name    : heap_sort.h
    Author       : Jun Zhang
	Mail         : ewalker.zj@gmail.com 
	Created Time : 2017-12-17 10-08
*****************************************************************/

#ifndef _HEAP_SORT_H
#define _HEAP_SORT_H

#include<iostream>
using namespace std;

void swap(int &a, int &b){
	int temp = a;
	a = b;
	b = temp;
}

void adjustHeap(int a[], int start, int end){
	int tmp = a[start];
	int idx = start, childIdx;
	while(2 * idx + 1 <= end){
		childIdx = 2 * idx + 1;
		// 2 * idx + 1, 2 * idx + 2
		if(childIdx < end && a[childIdx] < a[childIdx + 1])
			childIdx ++;

		if(tmp >= a[childIdx])
			break;
		else {
			a[idx] = a[childIdx];
			idx = childIdx;
		}
	}
	a[idx] = tmp;
}

void heap_sort(int a[], int len){
	for(int i = len / 2 - 1; i >= 0; i -- )
		adjustHeap(a, i, len - 1);

	for(int i = len - 1; i > 0; i --){
		swap(a[0], a[i]);
		adjustHeap(a, 0, i - 1);
	}
}
#endif
