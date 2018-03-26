/****************************************************************
    File Name    : call_once.cpp
    Author       : Jun Zhang
	Mail         : ewalker.zj@gmail.com 
	Created Time : 2018-03-04 14-33
*****************************************************************/

#include<iostream>
#include<thread>
#include<mutex>

std::once_flag flag;
int gid = 0;

inline void throw_func(bool do_throw){
    if(do_throw){
        std::cout << "throw func" << std::endl;
        throw std::exception();
    }
}

inline void do_func(bool do_throw){
    try{
        std::call_once(flag, throw_func, do_throw);
    } catch(...){

    }
}

int main(){
    std::thread t1(do_func, true);
    std::thread t2(do_func, true);
    std::thread t3(do_func, false);
    std::thread t4(do_func, true);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    
    std::cout << "gid: " << gid << std::endl;
    return 0;
}
