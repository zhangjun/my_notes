#include <cstdlib>
#include <iostream>

#include "merge_sort.h"
#include "shell_sort.h"

int partition_v1(int arr[], int begin, int end) {
  int pivot = arr[end];
  int i = begin, j = begin;
  while (j <= end - 1) {
    if (arr[j] <= pivot) {
      swap(arr[i++], arr[j]);
    }
    ++j;
  }
  swap(arr[i], arr[end]);
  std::cout << "pivot: " << pivot << std::endl;
  return i;
}

int partition_v2(int arr[], int begin, int end) {
  srand((unsigned)time(0));
  int rand_idx = rand() % (end - begin + 1) + begin;
  int povit = arr[rand_idx];
  swap(arr[begin], arr[rand_idx]);

  while (begin < end) {
    while (begin < end && arr[end] >= povit) {
      end--;
    }
    arr[begin] = arr[end];
    while (begin < end && arr[begin] <= povit) {
      begin++;
    }
    arr[end] = arr[begin];
  }
  arr[begin] = povit;
  return begin;
}

void quick_sort_routine(int arr[], int begin, int end) {
  if (begin < end) {
    // int idx = partition_v1(arr, begin, end);
    int idx = partition_v2(arr, begin, end);
    quick_sort_routine(arr, begin, idx - 1);
    quick_sort_routine(arr, idx + 1, end);
  }
}

void quick_sort(int arr[], int len) { quick_sort_routine(arr, 0, len - 1); }

int main() {
  int arr[12] = {4, 2, 3, 21, 19, 0, 1, 9, 11, 7, 4, 6};
  // quick_sort(arr, 12);
  // merge_sort(arr, 12);
  shell_sort_v2(arr, 12);
  for (int i = 0; i < 12; i++)
    std::cout << arr[i] << std::endl;

  return 0;
}
