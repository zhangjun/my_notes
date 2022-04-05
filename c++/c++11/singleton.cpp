/****************************************************************
    File Name    : singleton.cpp
    Author       : Jun Zhang
        Mail         : ewalker.zj@gmail.com
        Created Time : 2018-03-04 14-45
*****************************************************************/

#include <iostream>
using namespace std;

static std::once_flag oc;
Singleton *Singleton::m_instance;
Singleton *Singleton::getInstance() {
  std::call_once(oc, [&]() { m_instance = new Singleton(); });
  return m_instance;
}
