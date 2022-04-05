/****************************************************************
    File Name    : tt.cpp
    Author       : Jun Zhang
        Mail         : ewalker.zj@gmail.com
        Created Time : 2018-01-27 09-54
*****************************************************************/

#include <iostream>
using namespace std;

#define FOO(x)                                                                 \
  do {                                                                         \
    x++;                                                                       \
    x++;                                                                       \
  } while (0)

template <typename _T, typename _S>
auto multiply(_T t, _S s) -> decltype(t * s) {
  return t * s;
}

template <typename _T> auto multiply(_T t) -> decltype(t) { return t * 2; }

int main() {
  int b = 2;
  typedef decltype(b) IN;
  IN c = 213;
  cout << multiply(c) << endl;
  cout << multiply(b, c) << endl;
  int a = 0, n = 5;

  if (a == 0)
    FOO(n);
  else
    n--;
  cout << n << endl;
  return 0;
}
