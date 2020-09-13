#include <iostream>
#include <vector>

void compAndSwap(int a[], int i, int j, int dir) 
{ 
    if (dir==(a[i]>a[j])) 
        std::swap(a[i],a[j]); 
} 
  
/*It recursively sorts a bitonic sequence in ascending order, 
 *   if dir = 1, and in descending order otherwise (means dir=0). 
 *     The sequence to be sorted starts at index position low, 
 *       the parameter cnt is the number of elements to be sorted.*/
void bitonicMerge(int a[], int low, int cnt, int dir) 
{ 
    if (cnt>1) 
    { 
        int k = cnt/2; 
        for (int i=low; i<low+k; i++) 
            compAndSwap(a, i, i+k, dir); 
        bitonicMerge(a, low, k, dir); 
        bitonicMerge(a, low+k, k, dir); 
    } 
} 
  
/* This function first produces a bitonic sequence by recursively 
 *     sorting its two halves in opposite sorting orders, and then 
 *         calls bitonicMerge to make them in the same order */
void bitonicSort(int a[],int low, int cnt, int dir) 
{ 
    if (cnt>1) 
    { 
        int k = cnt/2; 
  
        // sort in ascending order since dir here is 1 
        bitonicSort(a, low, k, 1); 
        
        // sort in descending order since dir here is 0 
        bitonicSort(a, low+k, k, 0); 
                  
       // Will merge wole sequence in ascending order 
       // since dir=1. 
       bitonicMerge(a,low, cnt, dir); 
    } 
} 
                             
/* Caller of bitonicSort for sorting the entire array of 
   length N in ASCENDING order */
void sort(int a[], int N, int up) 
{ 
    bitonicSort(a,0, N, up); 
} 

int main() {
	std::vector<int> data{3, 1, 7, 4, 3, 9, 7, 3};
	sort(data.data(), data.size(), 1);
	for(auto &it : data)
		std::cout << it << std::endl;
	return 0;
}
