#pragma once 
#include <atomic>

class SpinLock {
public:
		SpinLock() : flag(std::ATOMIC_FLAG_INIT) {}
		
		SpinLock(const SpinLock& lock) = delete;
		SpinLock& operator= (const SpinLock& lock) = delete;

		void lock() { 
			while(flag.test_and_set(std::memory_order_acquire));
		}

		void unlock() {
			flag.clear(std::momory_order_release);
		}

private:
		std::atomic_flag flag;
};
