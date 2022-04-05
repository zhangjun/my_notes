/****************************************************************
    File Name    : t.cpp
    Author       : Jun Zhang
        Mail         : ewalker.zj@gmail.com
        Created Time : 2018-01-04 21-10
*****************************************************************/

#include <algorithm>
#include <iostream>
#include <string>
#include <tr1/unordered_map>
#include <vector>

using namespace std;
using namespace std::tr1;

struct cmp {
  size_t operator()(const int value) const { return value % 10; }
};

int main() {
  // unordered_map<int, int, less<int> > arr;
  unordered_map<int, int, cmp> arr;
  arr[23] = 2;
  arr[2] = 5;
  arr[4] = 3;
  arr[7] = 9;

  for (unordered_map<int, int>::iterator it = arr.begin(); it != arr.end();
       it++)
    cout << it->first << "-" << it->second << endl;

  return 0;
}
