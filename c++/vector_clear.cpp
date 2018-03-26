/****************************************************************
    File Name    : vector_clear.cpp
    Author       : Jun Zhang
	Mail         : zhangjun9@staff.weibo.com 
	Created Time : 2018-02-28 23-54
*****************************************************************/

#include<iostream>
#include<vector>
using namespace std;

int main(){
    vector<int> data;
    data.push_back(4);
    data.push_back(3);
    data.push_back(7);
    data.clear();
    vector<int>().swap(data);
    cout << data.size() << ":" << data.capacity() << endl;
    return 0;
}
