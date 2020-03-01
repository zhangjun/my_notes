#include <iostream>
#include <atomic>
#include <thread>

//std::atomic<int> num = 0;
//std::atomic_flag flag{std::ATOMIC_FLAG_INIT};

int data = 0;
std::atomic<int> guard(0);
void f1() {
	data = 12;
	guard.store(1, std::memory_order_release);
}

void f2() {
	//int val = guard.load(std::memory_order_acquire);
	int val = guard.load(std::memory_order_consume);
	if(val != 0) {
		int g = data;
		std::cout << "g: " << g << std::endl;
	}
}

int main () {
	std::thread t1(f1);
	std::thread t2(f2);

	t1.join();
	t2.join();
	return 0;
}
