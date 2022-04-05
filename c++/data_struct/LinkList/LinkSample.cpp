/****************************************************************
    File Name    : LinkSample.cpp
    Author       : Jun Zhang
        Mail         : ewalker.zj@gmail.com
        Created Time : 2018-03-17 21-23
*****************************************************************/

#include <iostream>
using namespace std;

#include "LinkList.h"

bool IsExistLoop(LinkList *head) {
  LinkList *slow = head, *fast = head;
  while (fast && fast->next) {
    slow = slow->next;
    fast = fast->next->next;
    if (slow == fast)
      break;
  }

  if (fast == NULL || fast->next == NULL)
    return false;
  else
    return true;
}

// 2(d + a) = (d + a) + nr  =》 d = nr -a
// d + r = L =》 r = L - d
// d = (n-1)r + L - d - a
LinkList *LoopPort(LinkList *head) {
  LinkList *slow = head, *fast = head;
  while (fast && fast->next) {
    slow = slow->next;
    fast = fast->next->next;
    if (slow == fast)
      break;
  }

  if (fast == NULL || fast->next == NULL)
    return NULL;

  slow = head;
  while (slow != fast) {
    slow = slow->next;
    fast = fast->next->next;
  }

  return slow;
}

int main() {
  int n = 5;
  LinkList *link = (LinkList *)malloc(sizeof(LinkList) * n);

  link[0].val = 2;
  link[0].next = &link[1];

  link[1].val = 2;
  link[1].next = &link[2];

  link[2].val = 2;
  link[2].next = &link[3];

  link[3].val = 2;
  link[3].next = &link[4];

  link[4].val = 2;
  link[4].next = &link[2];

  if (IsExistLoop(link))
    cout << "exist loop" << endl;
  else
    cout << "do not exist loop" << endl;

  return 0;
}
