/****************************************************************
    File Name    : class.cpp
    Author       : Jun Zhang
	Mail         : ewalker.zj@gmail.com 
	Created Time : 2017-12-29 18-23
*****************************************************************/

#include<iostream>
using namespace std;

#pragma pack(show)

class Test {
    private:
        char c;
    public:
        Test() {}
        Test(char ch): c(ch){} 
        void print();
};

void Test::print(){
    cout << c << endl;
}

class A {
    public:
        A() {}
        A(int i) {}
        virtual void print(){
            cout << "base" << endl;
        }
};

class B : public A {
    public:
        virtual void print(){
            cout << "derived" << endl;
        }
    B() {}
    A a;
};

int main(){
    Test t('e');
    t.print();
    cout << sizeof(t) << endl;
    B b;
    A a = b;
    a.print();

    A& a1 = b;
    a1.print();
    
    A *a2 = &b;
    a2 -> print();
    return 0;
}

