#include <mutex>
#include "lock_guard.h"

int main() {
    std::mutex mtx;
    LockGuard<std::mutex> lck(mtx);
    return 0;
}
