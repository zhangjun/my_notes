#pragma once 

#include <iostream>

template <typename T>
class LockGuard {
public:
    LockGuard(T &mtx): mtx_(mtx) {
        mtx_.lock();
    }
    ~LockGuard() {
        mtx_.unlock();
    }
    LockGuard(const LockGuard<T>&) = delete;
    LockGuard<T>& operator= (const LockGuard<T>&) = delete;

private:
    T& mtx_;
};
