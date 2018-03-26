/****************************************************************
    File Name    : thread_pool.cpp
    Author       : Jun Zhang
	Mail         : ewalker.zj@gmail.com 
	Created Time : 2018-03-04 15-40
*****************************************************************/

#include "thread_pool.h"
#include <cassert.h>

ThreadPool::ThreadPool(const std::string &name) :
    _name(name),
    _maxQueueSize(0),
    _running(false)
{
}

ThreadPool::~ThreadPool(){
    if(_running){
        Stop();
    }
}

void ThreadPool::Start(int num_thread){
    assert(_threads.empty());
    _running = true;
    _threads.reserve(num_thread);

    for(int i = 0; i < num_thread; ++ i){
        _threads.push_back(std::thread(&ThreadPool::RunInThread, this));
    }
}

void ThreadPool::Stop(){
    {
        unique_lock<mutex> lock(_mutex);
        _running = false;
        _notEmpty.notify_all();
    }

    for(size_t i = 0; i < _threads.size(); ++ i){
        _threads[i],join();
    }
}

void ThreadPool::Run(const Task &t){
    // single thread
    if(_threads.empty()){
        t();
    } else {
        unique_lock<mutex> lock(_mutex);
        while(IsFull()){
            _notFull.wait(lock);
        }
        assert(!IsFull());
        _queue.push_back(t);
        _notEmpty.notify_one();
    }
}

ThreadPool::Task ThreadPool::Take(){
    unique_lock<mutex> lock(_mutex);
    while(_queue.empty() && _running)
        _notEmpty.wait(lock);

    Task task;
    if(!_queue.empty()){
        task = _queue.front();
        _queue.pop_front();
        if(_maxQueueSize > 0){
            _notFull.notify_one();
        }
    }
    return task;
}

bool ThreadPool::IsFull(){
    return _maxQueueSize > 0 && _queue.size() >= _maxQueueSize;
}

void ThreadPool::RunInThread(){
    try{
        while(_running){
            Task task = take();
            if(task){
                task();
            }
        }
    }
    catch(const std::exception& ex){
        std::err << "exception caught in ThreadPool " << _name << std::endl;
        std::err << "exception info: " << ex.what() << std::endl;
        std::abort();
    }
    catch(...){
        std::err << "caught exception in ThreadPool %s" << _name << std::endl;
    }
}
