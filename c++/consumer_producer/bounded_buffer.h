#include <mutex>
#include <condition_variable>
#include <memory>

struct BoundedBuffer {
    BoundedBuffer(int capacity): capacity_(capacity), front_(0), rear_(0), count_(0) {
        buffer_ = new int[capacity_];
    }

    ~BoundedBuffer() {
        if(buffer != nulltpr) {
            delete [] buffer_;
            buffer_ = nullptr;
        }
    }

    void push(int data) {
        std::unique_lock<std::mutex> mtx(lock_);
        not_full_.wait(mtx, [this]() { return count_ != capacity_; });
        buffer_[rear_] = data;
        rear_ = (rear_ + 1) % capacity_;
        ++ count_;
        mtx.unlock();
        not_empty_.notify_one();
    }

    void get() {
        std::unique_lock<std::mutex> mtx(lock_);
        not_empty_.wait(mtx, [this]() { return count_ != 0; });
        int result = buffer_[front_];
        front_ = (front_ + 1) % capacity_;
        ++ count_;
        mtx.unlock();
        not_full_.notify_one();
        return result;
    }

    std::mutex lock_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;
    int capacity_;
    int count_;
    int front_;
    int rear_;

    int* buffer_;
};
