/****************************************************************
    File Name    : nocopyable.h
    Author       : Jun Zhang
	Mail         : ewalker.zj@gmail.com 
	Created Time : 2018-03-04 15-19
*****************************************************************/

#ifdef _NO_COPYABLE_H
#define _NO_COPYABLE_H

class NoCopyable{
    private:
        NoCopyable(const NoCopyable& x) = delete;
        NoCopyable& operaton= (const NoCopyable& x) = delete;
    public:
        NocopyAble() = default;
        ~NocopyAble() = default;
};

#endif

