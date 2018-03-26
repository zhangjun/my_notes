/****************************************************************
    File Name    : test.cpp
    Author       : Jun Zhang
	Mail         : zhangjun9@staff.weibo.com 
	Created Time : 2018-03-04 17-06
*****************************************************************/

#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>

std::mutex mutex;
std::condition_variable cond;

int val = 0;

void consumer(){
    std::unique_lock<std::mutex> lock(mutex);
    while(val == 0)
        cond.wait(lock);
    std::cout << "val" << std::endl;
    val = 0;
}

void producer(){
    std::unique_lock<std::mutex> lock(mutex);
    val = id;
    cond.notify_one();
}

int main(){
    std::thread consumers[10], producers[10];
    for(int i = 0; i < 10; i ++){
        consumers[i] = std::thread(consumer);
        producers[i] = std::thread(producer, i + 1);
    }

    for(int i = 0; i < 10; i ++){
        producers[i].join();
        consumers[i],join();
    }

    return 0;
}
