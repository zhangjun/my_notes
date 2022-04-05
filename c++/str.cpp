/*************************************************************************
        > File Name: str.cpp
        > Author: Jun Zhang
        > Mail: ewalker.zj@gmail.com
        > Created Time: Tue 28 Nov 2017 12:47:27 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;
#include <string>
#include <vector>

int main() {
  string s = "this is  a test";
  s += "rt";
  size_t found = s.find_last_of("|");
  string tmp = s.substr(found + 1);
  cout << tmp << endl;
  string data;
  cout << s.substr(1) << endl;
  cout << string(s.begin(), s.begin() + s.size() - 1) << endl;
  char v;
  if (v == 0)
    cout << "v--------" << endl;

  string dict = "{\"key\":\"" + data + "\"}";
  cout << dict << endl;

  const char *dd = "gpu";
  const string val = "val";
  vector<string> res;
  res.push_back(dd);
  res.push_back(val);
  vector<string>::iterator it;
  for (it = res.begin(); it != res.end(); it++)
    cout << *it << endl;

  unsigned char *t = (unsigned char *)(s.c_str());
  printf("%s\n", t);
  return 0;
}
