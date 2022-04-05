#include <iostream>
#include <future>
#include <thread>
#include <chrono>

void run(int val) {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "run tid: " << std::this_thread::get_id() << ", val: " << val << std::endl;
}

struct runner {
};

int main() {
    std::cout << "main thread, tid: " << std::this_thread::get_id() << std::endl;
    std::future<void> future = std::async(std::launch::async, run, 4);
    future.get();

    std::future<void> f = std::async(std::launch::async, run, 5);
    std::future_status status = f.wait_for(std::chrono::seconds(2));
    if(status == std::future_status::timeout) {
        std::cout << "timeout." << std::endl;
    }
    return 0;
}