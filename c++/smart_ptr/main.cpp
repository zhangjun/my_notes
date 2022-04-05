/****************************************************************
    File Name    : main.cpp
    Author       : Jun Zhang
        Mail         : ewalker.zj@gmail.com
        Created Time : 2018-03-24 20-04
*****************************************************************/

#include "smart_ptr.h"
#include <iostream>

int main() {

  int *data = new int(3);

  SmartPtr<int> a(data);

  SmartPtr<int> b(a);

  SmartPtr<int> c(new int(5));
  std::cout << c.GetRefCount() << std::endl;
  c = a;

  std::cout << a.GetRefCount() << std::endl;
  std::cout << b.GetRefCount() << std::endl;
  std::cout << c.GetRefCount() << std::endl;
  return 0;
}
