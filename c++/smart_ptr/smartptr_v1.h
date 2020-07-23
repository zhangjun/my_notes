#pragma once

template <typename T>
class Deleter {
    void operator()(T* &ptr) {
        if(ptr) {
            delete ptr;
            ptr = nullptr;
        }
    }
};

// threadsafe ptr
template <typename T, typename D = Deleter<T>>
class shared_ptr {
public:
    shared_ptr(T* ptr = nullptr): ptr_(ptr), ref_cnt_(nullptr), mtx_(nullptr) {
        if(ptr_) {
            ref_cnt_ = new int(1);
            mtx_ = new mutex;
        }
    }
    ~shared_ptr() {
        release();
    }
    shared_ptr(const shared_ptr<T>& sp): ptr_(sp.ptr_), ref_cnt_(p.ref_cnt_), mtx_(p.mtx_) {
        inc_ref();
    }

    shared_ptr<T>& operator= (const shared_ptr<T>& sp) {
        if(this == &sp)
            return *this;

        if(ptr_ && 0 == desc_ref()) {
            delete ptr_;
            delete ref_cnt_;
        }

        ptr_ = sp.ptr_;
        ref_cnt_ = sp.ptr_;
        inc_ref()
        return *this;
    }
    T& operator*() {
        return *ptr_;
    }
    T* operator->() {
        return ptr_;
    }
    int use_count() {
        return *ref_cnt_;
    }
private:
    void inc_ref() {
        mtx_ -> lock();
        ++ *ref_cnt_;
        mtx_ -> unlock();
    }
    int sub_ref() {
        mtx_ -> lock();
        -- *ref_cnt_;
        mtx_ -> unlock();
        return *ref_cnt;
    }
    void release() {
        if(_ptr && 0 == sub_ref()) {
            D()(ptr_);
            delete ref_cnt_;
            if(mtx_)
                delete mtx_;
        }
    }
private:
    T* ptr_;
    int* ref_cnt_;
    mutex* mtx_;
};

