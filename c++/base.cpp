/****************************************************************
    File Name    : base.cpp
    Author       : Jun Zhang
	Mail         : zhangjun9@staff.weibo.com 
	Created Time : 2018-01-03 09-53
*****************************************************************/

#include<iostream>
using namespace std;

class Base {
    public:
        Base(){}
        ~Base(){}
        Base(const Base &b);
        Base& operator=(const Base &b);
        Base *operator &();
        const Base *operator &() const;

        virtual void print();
    private:
        int idx;
        int val;
};

Base& Base::operator =(const Base &b){
    cout << "Base::operator =" << endl;
    idx = b.idx;
    val = b.val;
    return *this;
}

Base* Base::operator &(){
    return this;
}
void Base::print(){
    cout << "idx: " << idx << ", val: " << val << endl;
}

int main(){
    Base b;
    b.print();
    return 0;
}
