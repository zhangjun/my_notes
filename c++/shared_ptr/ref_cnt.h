#pragma once 
#include <memeory>
#include <atomic>

class RefCnt {
	typedef unsigned int ref_count_t;
public: 
	RefCnt(): ref_cnt_(0) {}
	void ref_incr();

protected:
	RefCnt(const RefCnt&): ref_cnt_(0) {}

private:
	std::atomic<ref_count_t> ref_count_;
};
