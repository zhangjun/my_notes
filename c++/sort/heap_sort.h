/****************************************************************
    File Name    : heap_sort.h
    Author       : Jun Zhang
	Mail         : ewalker.zj@gmail.com 
	Created Time : 2017-12-17 10-08
*****************************************************************/

#ifndef _HEAP_SORT_H
#define _HEAP_SORT_H

#include<iostream>
#include "common.h"
using namespace std;

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
    // build heap, top is max
	for(int i = len / 2 - 1; i >= 0; i -- )
		adjustHeap(a, i, len - 1);

    //adjust from root, One by one extract an element from heap root
	for(int i = len - 1; i > 0; i --){
        // Move current root to end 
		swap(a[0], a[i]);
        // call max heapify on the reduced heap 
		adjustHeap(a, 0, i - 1);
	}
}
#endif
