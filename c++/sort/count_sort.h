/*************************************************************************
   File Name    : count_sort.h
   Author       : Jun Zhang
   Email        : ewalker.zj@gmail.com
   Created Time : 2020-02-17 07:28
   Description  : 
*************************************************************************/

// limit range sort, ex. char sort

#define RANGE 10  // for example
#define MINVAL 0
#define MAXVAL 9

void count_sort(int arr[], int n) {
    int output[n];

    // init count
    int count* = int[range + 1];
    for(int i = 0; i < RANGE + 1; i ++) {
        count[i] = 0;
    }

    for(int i = 0; i < n; i ++) {
        count[arr[i] - MINVAL] ++;
    }

    for(int i = 1; i <= RANGE; i ++) 
        count[i] += count[i - 1];

    for(i = n - 1; i > 0; i --) {
        output[count[arr[i] - MINVAL] - 1] = arr[i];
        -- count[arr[i] - MINVAL];
    }

    // copy output to arr
    for(int i = 0; i < n; i ++) {
        arr[i] = output[i];
    }
}

