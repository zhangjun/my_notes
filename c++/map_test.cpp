/****************************************************************
    File Name    : map_test.cpp
    Author       : Jun Zhang
	Mail         : ewalker.zj@gmail.com 
	Created Time : 2018-02-09 14-29
*****************************************************************/

#include<iostream>
#include<string>
#include<tr1/unordered_map>

using namespace std;

int main(){
    tr1::unordered_map<string, int> data;
    data["first"] = 12;
    data["second"] = 13;
    data["third"] = 14;

    tr1::unordered_map<string, int> test;
    test = data;
    tr1::unordered_map<string, int>::iterator it;
    for(it = test.begin(); it != test.end(); it ++){
        cout << it -> first << "-" << it -> second << endl;
    }

    tr1::unordered_map<string,
    return 0;
}
