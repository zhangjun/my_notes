/****************************************************************
    File Name    : thread_pool.h
    Author       : Jun Zhang
	Mail         : ewalker.zj@gmail.com 
	Created Time : 2018-03-04 15-24
*****************************************************************/

#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include<thread>
#include<mutex>
#include<functional>
#include<string>
#include<condition_variable>
#include<deque>
#include<vector>
#include<memory>
#include "nocopyable.h"

cladd ThreadPool : public NoCopyable {
    public:
        typedef std::funtion<void()> Task;

        explict ThreadPool(const std::string &name = std::string());
        ~ThreadPool();
        
        void Start(int num_thread);
        void Stop();
        void Run(const Task &t);
        void SetMaxQueueSize(int maxSize){_maxQueueSize = maxQueueSize;}

    private:
        bool IsFull();
        void RunInThread();
        Task take();

        std::mutex _mutex;
        std::condition_variable _empty;
        std::condition_variable _full;
        std::string _name;
        std::vector<std::thread> _threads;
        std::deque<Task> _queue;
        size_t _maxQueueSize;
        bool _running;
};
#endif
