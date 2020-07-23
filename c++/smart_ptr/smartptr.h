#pragma once

template <typename T>
class shared_ptr {
public:
    shared_ptr(T* ptr = nullptr): ptr_(ptr), ref_cnt_(nullptr) {
        if(ptr_) {
            ref_cnt_ = new int(1);
        }
    }
    ~shared_ptr() {
        if(_ptr && 0 == --*ref_cnt_) {
            delete ptr_;
            delete ref_cnt_;
        }
    }
    shared_ptr(const shared_ptr<T>& sp): ptr_(sp.ptr_), ref_cnt_(p.ref_cnt_) {
        if(ptr_) {
            ++ *ref_cnt_;
        }
    }

    shared_ptr<T>& operator= (const shared_ptr<T>& sp) {
        if(this == &sp)
            return *this;

        if(ptr_ && 0 == -- *ref_cnt_) {
            delete ptr_;
            delete ref_cnt_;
        }

        ptr_ = sp.ptr_;
        ref_cnt_ = sp.ptr_;
        if(ptr_) {
            ++ *ref_cnt_;
        }
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
    T* ptr_;
    int* ref_cnt_;
};


// threadsafe ptr
template <typename T>
class shared_ptr {
public:
    shared_ptr(T* ptr = nullptr): ptr_(ptr), ref_cnt_(nullptr) {
        if(ptr_) {
            ref_cnt_ = new int(1);
        }
    }
    ~shared_ptr() {
        if(_ptr && 0 == --*ref_cnt_) {
            delete ptr_;
            delete ref_cnt_;
        }
    }
    shared_ptr(const shared_ptr<T>& sp): ptr_(sp.ptr_), ref_cnt_(p.ref_cnt_) {
        if(ptr_) {
            ++ *ref_cnt_;
        }
    }

    shared_ptr<T>& operator= (const shared_ptr<T>& sp) {
        if(this == &sp)
            return *this;

        if(ptr_ && 0 == -- *ref_cnt_) {
            delete ptr_;
            delete ref_cnt_;
        }

        ptr_ = sp.ptr_;
        ref_cnt_ = sp.ptr_;
        if(ptr_) {
            ++ *ref_cnt_;
        }
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
    T* ptr_;
    int* ref_cnt_;
};
