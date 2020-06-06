#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

mutex mtx;
condition_variable produce, consume;

queue<int> q;
int cap = 1024;

void consumer() {
    while(true) {
        unique_lock<mutex> lck(mtx);
        consume.wait(lck, [] {return q.size() != 0; });

        std::cout << "[consume][" << this_thread::get_id() << "] " << q.front() << ", " << q.size() << "\n";
        q.pop();
        produce.notify_all();
    }
}

void producer(int id) {
    while(true) {
        unique_lock<mutex> lck(mtx);
        produce.wait(lck, [] {return q.size() != cap; });

        std::cout << "[produce][" << this_thread::get_id() << "] " << id << ", " << q.size() << "\n";
        q.push(id);
        consume.notify_all();
    }
}

int main() {
    constexpr int consumer_num = 4, producer_num = 6;
    std::vector<std::thread> consumers, producers;
    for(int i = 0; i < consumer_num; i ++) {
        consumers.emplace_back(thread(consumer));
    }

    for(int i = 0; i < producer_num; i ++) {
        producers.emplace_back(thread(producer, i + 1));
    }

    for(auto &it: consumers) it.join();
    for(auto &it: producers) it.join();
    return 0;
}
