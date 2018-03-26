/****************************************************************
    File Name    : smart_ptr.h
    Author       : Jun Zhang
	Mail         : ewalker.zj@gmail.com 
	Created Time : 2018-03-24 19-47
*****************************************************************/

#ifndef _SMART_PTR_H
#define _SMART_PTR_H
#include<iostream>

template<typename T>
class SmartPtr;

template<typename T>
class PtrHandle {
    private:
        friend class SmartPtr<T>;

        PtrHandle(T *ptr) : p(ptr), count(1) {}
        ~PtrHandle() { delete p;}

        int count;
        T *p;
};

template<typename T>
class SmartPtr {
    public:
        SmartPtr(T *ptr) : rp(new PtrHandle<T>(ptr)) {}
        SmartPtr(const SmartPtr<T> &sp) : rp(sp.rp) { ++ rp -> count; }
        SmartPtr& operator= (const SmartPtr<T>& rhs) {
            ++ rhs.rp -> count;
            if(-- rp -> count == 0){
                std::cout << "delete rp, ref count: " << rp -> count << std::endl;
                delete rp;
            }

            rp = rhs.rp;
            return *this;
        }

        T& operator*(){
            return *(rp -> p);
        }

        T* operator -> (){
            return rp -> p;
        }

        const int GetRefCount(){
            return rp -> count;
        }

        ~SmartPtr(){
            if( -- rp -> count == 0){
                std::cout << "delete rp, ref count: " << rp -> count << std::endl;
                delete rp;
            }
            else
                std::cout << "ref count: " << rp -> count << std::endl;
        }

    private:
        PtrHandle<T> *rp;
};
#endif
