/****************************************************************
    File Name    : t.cpp
    Author       : Jun Zhang
	Mail         : ewalker.zj@gmail.com 
	Created Time : 2017-12-18 23-38
*****************************************************************/

#include<iostream>
using namespace std;
#include "quick_sort.h"


int main(){
    int arr[9] = {4, 2, 3, 1, 9, 11, 7, 4, 6};
    quick_sort(arr, 9);
    for(int i = 0; i < 9; i++)
        cout << arr[i] << endl;

    return 0;
}
