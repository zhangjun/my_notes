#include <iostream>

#include "thread_pool.h"

struct sum {
  int operator()(int a, int b) {
      int res = a + b;
      return res;
  }
};

int print(int a) {
    return a;
}

class A {
public:
  static int calc(int val) {
      return val;
  }
};
int main() {
  ThreadPool executor(10);
  auto result = executor.Commit(&print, 3);
  std::cout << "result: " << result.get() << std::endl;

  // auto result = executor.Commit(A::calc, 3);
  // std::cout << "result: " << result.get() << std::endl;
  ThreadPool pool(4);
  std::vector<std::future<int>> results;
  std::chrono::seconds span(1);
  for (int i = 0; i < 2; ++ i) {
    results.emplace_back(
        pool.Commit([i, span] {
            std::cout << "run " << i << std::endl;
            std::this_thread::sleep_for(span);
            return i * i;
        })
    );
  }

  for (auto && item : results) {
      if(item.wait_for(span) == std::future_status::ready) {
        std::cout << item.get() << std::endl;
      }
  }

  return 0;
}