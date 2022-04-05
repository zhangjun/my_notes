/****************************************************************
    File Name    : ptr_test.cpp
    Author       : Jun Zhang
        Mail         : ewalker.zj@gmail.com
        Created Time : 2018-03-11 22-00
*****************************************************************/

#include <iostream>
#include <memory>
using namespace std;

int main() {
  shared_ptr<int> p = make_shared<int>(1);
  // p = make_shared<int>(6);
  shared_ptr<int> q = p;
  shared_ptr<int> tmp;
  tmp.reset(p);
  // cout << *p << endl;
  cout << p.use_count() << endl;
  cout << q.use_count() << endl;
  cout << tmp.use_count() << endl;
  cout << p.unique() << endl;
  return 0;
}
