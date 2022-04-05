/*************************************************************************
        > File Name: vector_test.cpp
        > Author: Jun Zhang
        > Mail: ewalker.zj@gmail.com
        > Created Time: Mon 11 Dec 2017 04:58:04 PM CST
 ************************************************************************/

#include "topic_util.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
  // string s = "this is # test#vector # ";
  string s = "##vector#";
  vector<string> buf;
  vector<string>::iterator it;
  split(s, "#", buf);

  cout << "len: " << buf.size() << endl;
  for (it = buf.begin(); it != buf.end(); it++) {
    cout << *it << endl;
  }

  cout << "buf size: " << buf.size() << endl;

  string data = "deep # gpu#cuda # # # hello##";
  split(data, "#", buf);
  cout << "buf size: " << buf.size() << endl;
  cout << buf[2] << endl;
  for (it = buf.begin(); it != buf.end(); it++) {
    if (!(*it).empty())
      cout << *it << endl;
  }

  buf.clear();
  buf.push_back("test");
  it = buf.begin();
  cout << *it << endl;
  it = buf.end() - 1;
  cout << *it << endl;
  cout << buf.end() - buf.begin() << endl;
  cout << buf.size() << endl;
  return 0;
}
