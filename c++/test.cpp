/****************************************************************
    File Name    : test.cpp
    Author       : Jun Zhang
	Mail         : ewalker.zj@gmail.com 
	Created Time : 2018-01-04 17-11
*****************************************************************/

#include<iostream>
#include<string>
#include<vector>
#include "topic_util.h"

using namespace std;

int main(){
    string data = "this, is, , data";
    vector<string> arr;
    split(data, ",", arr);
    vector<string>::iterator it;
    for(it = arr.begin(); it != arr.end(); it ++){
        size_t first = (*it).find_first_not_of(" ");
        size_t end = (*it).find_last_not_of(" ");
        if(first == string::npos)
            continue;
        
        string param = (*it).substr(first, end - first + 1);
        cout << "first: " << first << ", second: " << end << ", param: " << param << ", size: " << param.size() << endl;
    }

    return 0;
}
