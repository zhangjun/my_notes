#include "lock_guard.h"
#include <mutex>

int main() {
  std::mutex mtx;
  LockGuard<std::mutex> lck(mtx);
  return 0;
}
