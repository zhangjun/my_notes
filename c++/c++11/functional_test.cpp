/****************************************************************
    File Name    : functional_test.cpp
    Author       : Jun Zhang
        Mail         : ewalker.zj@gmail.com
        Created Time : 2018-03-23 23-32
*****************************************************************/

#include <functional>
#include <iostream>

using namespace std;

int foo(int val) { return val; }

int too(int i, int b) { return i + b; }

int main() {
  std::function<int(int)> f = foo;
  int v = 19;
  cout << f(v) << endl;

  v = 3;
  function<int(int)> func = [&](int val) -> int { return 1 + val * 2; };

  cout << func(v) << endl;

  cout << bind<int>(too, _1, _2)(3, 4) << endl;
  return 0;
}
