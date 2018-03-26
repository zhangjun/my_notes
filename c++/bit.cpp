/****************************************************************
    File Name    : bit.cpp
    Author       : Jun Zhang
	Mail         : zhangjun9@staff.weibo.com 
	Created Time : 2017-12-30 12-16
*****************************************************************/

#include<iostream>
using namespace std;

int main(){
    int n = 23;
    while(n != 0){
        cout << (n & 0x1) << endl;
        n >>= 1;
    }
    return 0;
}
