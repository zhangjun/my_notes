/****************************************************************
    File Name    : call_once.cpp
    Author       : Jun Zhang
	Mail         : zhangjun9@staff.weibo.com 
	Created Time : 2018-03-04 14-33
*****************************************************************/

#include<iostream>
#include<thread>
#include<mutex>

std::once_flag flag;
int gid = 0;

void func(std::string param){
    gid ++;
    std::cout << "call once! " << gid << ", param: " << param << std::endl;
}

void print_func(const std::string &param){
    std::call_once(flag, func, param);
}

int main(){
    std::thread t1(print_func, "1");
    std::thread t2(print_func, "2");
    std::thread t3(print_func, "3");
    std::thread t4(print_func, "4");

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    
    std::cout << "gid: " << gid << std::endl;
    return 0;
}
