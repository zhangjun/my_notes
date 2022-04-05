#include <atomic>
#include <cassert>
#include <string>
#include <thread>

std::atomic<bool> ready{false};
int data = 0;

void producer() {
  data = 100;
  ready.store(true, std::memory_order_release);
}

void consumer() {
  while (!ready.load(std::memory_order_acquire))
    ;
  assert(data == 100);
}

int main() {
  std::thread f(producer);
  std::thread e(consumer);
  f.join();
  e.join();
  return 0;
}
