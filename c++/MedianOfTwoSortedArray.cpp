/****************************************************************
    File Name    : MedianOfTwoSortedArray.cpp
    Author       : Jun Zhang
	Mail         : zhangjun9@staff.weibo.com 
	Created Time : 2018-01-02 15-14
*****************************************************************/

#include<iostream>
#include<vector>
#include<algorithm>
#include<climits>

using namespace std;

class Routine {
    public:
        double MedianOfTwoSortedArray(vector<int> &first, vector<int> &second){
            double median_first;
            int first_len = first.size(), second_len = second.size();
            if(first_len + second_len == 0)
                return 0;

            int first_idx = 0, second_idx = 0;
            while(first_idx + second_idx != (first_len + second_len + 1) / 2){
                int a = (first_idx == first_len) ? INT_MAX : first[first_idx];
                int b = (second_idx == second_len) ? INT_MAX : second[second_idx];
                if(a < b){
                    median_first = a;
                    first_idx ++;
                }
                else {
                    median_first = b;
                    second_idx ++;
                }
            }

            if((first_len + second_len) % 2 == 1)
                return median_first;
            else {
                int temp_a = (first_idx == first_len) ? INT_MAX : first[first_idx];
                int temp_b = (second_idx == second_len) ? INT_MAX : second[second_idx];
                double median_second = (temp_a < temp_b) ? temp_a : temp_b;
                return (median_first + median_second) / 2;
            }
        }

};

int main(){
    Routine r;
    vector<int> first;
    vector<int> second = {9, 2, 3, 6, 2, 10};
    sort(first.begin(), first.end());
    sort(second.begin(), second.end());

    for(auto i : first)
        cout << i << endl;
    cout << "================" << endl;

    for(auto i : second)
        cout << i << endl;
    cout << "================" << endl;

    cout << r.MedianOfTwoSortedArray(first, second) << endl;
    return 0;
}
