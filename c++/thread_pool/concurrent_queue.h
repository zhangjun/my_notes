#pragma once 

#include <mutex>
#include <condition_variable>
#include <thread>
#include <list>

#include <chrono>

#include <iostream>

#define MAX_SIZE 5000

class wait_list_t {
public:
	wait_list_t() : _alive(1), _num(0) {
		
	}
	
	~wait_list_t() {
	}

	int len() {
		std::unique_lock<std::mutex> lk(_mutex);
		return _num;
	}

	void put(void* node) {
		std::unique_lock<std::mutex> lk(_mutex);
		if (_alive && _num < MAX_SIZE) {
			nodes.push_back(node);
			++ _num;
		}
		lk.unlock();
		_cond.notify_one();
	}

	void put_front(void* node) {
		std::unique_lock<std::mutex> lk(_mutex);
		if (_alive && _num < MAX_SIZE) {
			nodes.push_front(node);
			++ _num;
		}
		lk.unlock();
		_cond.notify_one();
	}
	
	bool try_get_from_head(void* node) {
		node = NULL;
		std::unique_lock<std::mutex> lk(_mutex);
		if( !_alive || _num == 0 ) {
			return false;
		}
		node = nodes.front();
		nodes.pop_front();
	        -- _num;
		return true;
	} 
	
	void* get_from_head() {
		void* node = NULL;
		std::unique_lock<std::mutex> lk(_mutex);
		while( _alive && _num == 0 ) {
			_cond.wait(lk);
			//_cond.wait_for(lk, std::chrono::milliseconds(5), []{return;});
		}
		if (_alive && !nodes.empty()) {
			//std::cout << "nodes size: " << nodes.size() << std::endl;
			node = nodes.front();
			nodes.pop_front();
			-- _num;
		} else {
			node = NULL;
		}
		return node;
	} 
	
	void flush() {
		std::unique_lock<std::mutex> lk(_mutex);
		_alive = 0;
		lk.unlock();
		_cond.notify_all();
	}
private:
	std::list<void*> nodes;
	std::mutex _mutex;
	std::condition_variable _cond;
	int _alive;
	int _num;
};
