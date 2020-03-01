#pragma once 
#include <thread>
#include <mutex>
#include <condition_variable>

class RWLock {
public:
	RWLock() = default;
	~ RWLock() = default;
	
	void lock_read() {
		std::unique_lock<std::mutex> lck(mutex_);
		read_cond_.wait(lck, [=]() -> bool { return write_cnt_ == 0;});
		++ read_cnt_;
	}
	
	void lock_write() {
		std::unique_lock<std::mutex> lck(mutex_);
		++ write_cnt_;
		write_cond_.wait(lck, [=]() -> bool { return read_cnt_ == 0 && !write_flag_;});
		write_flag_ = true;
	}
	
	void unlock_read() {
		std::unique_lock<std::mutex> lck(mutex_);
		if( -- read_cnt_ == 0 && write_cnt_ > 0) {
			write_cond_.notify_one();
		}
	}
	
	void unlock_write() {
		std::unique_lock<std::mutex> lck(mutex_);
		if( -- write_cnt_ == 0) {
			read_cond_.notify_all();
		} else {
			read_cond_.notify_one();
		}
		write_flag_ = false;
	}

private:
	volatile int read_cnt_{0};
	volatile int write_cnt_{0};
	volatile bool write_flag_{false};
	
	std::mutex mutex_;
	std::condition_variable read_cond_;
	std::condition_variable write_cond_;
}
