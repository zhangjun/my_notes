#include <pthread.h>
#include <stdio.h>

#include <chrono>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

pthread_mutex_t p_mutex;
std::mutex mutex;

void p_mutex_func() {
  pthread_mutex_lock(&p_mutex);
  pthread_mutex_unlock(&p_mutex);
}

void mutex_func() { std::unique_lock<std::mutex> lk(mutex); }

void *p_mutex_loop(void *) {
  int num_iter = 100000;
  for (int i = 0; i < num_iter; i++) {
    pthread_mutex_lock(&p_mutex);
    pthread_mutex_unlock(&p_mutex);
  }
}

void mutex_loop() {
  int num_iter = 100000;
  for (int i = 0; i < num_iter; i++) {
    std::unique_lock<std::mutex> lk(mutex);
  }
}

void p_mutex_multi() {
  int num_thread = 40;
  pthread_t pid[num_thread];

  for (int i = 0; i < num_thread; i++) {
    pthread_create(&pid[i], NULL, p_mutex_loop, NULL);
  }

  for (int i = 0; i < num_thread; i++) {
    pthread_join(pid[i], NULL);
  }
}

void mutex_multi() {
  int num_thread = 40;
  std::vector<std::thread> worker;
  for (int i = 0; i < num_thread; i++) {
    worker.emplace_back(std::thread(mutex_loop));
  }

  for (int i = 0; i < num_thread; i++) {
    worker[i].join();
  }
}

int main() {

  pthread_mutex_init(&p_mutex, NULL);

  int n = 100000;
  auto begin = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < n; i++) {
    p_mutex_func();
  }
  auto end = std::chrono::high_resolution_clock::now();
  long long cost1 =
      std::chrono::duration_cast<std::chrono::microseconds>(end - begin)
          .count();

  begin = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < n; i++) {
    mutex_func();
  }
  end = std::chrono::high_resolution_clock::now();
  long long cost2 =
      std::chrono::duration_cast<std::chrono::microseconds>(end - begin)
          .count();

  printf("pthread mutex cost: %lld, mutex cost: %lld\n", cost1, cost2);
  printf("pthread mutex avgcost: %.06f, mutex avgcost: %.06f\n",
         cost1 / float(n), cost2 / float(n));

  begin = std::chrono::high_resolution_clock::now();
  p_mutex_multi();
  end = std::chrono::high_resolution_clock::now();
  long long cost3 =
      std::chrono::duration_cast<std::chrono::microseconds>(end - begin)
          .count();

  begin = std::chrono::high_resolution_clock::now();
  mutex_multi();
  end = std::chrono::high_resolution_clock::now();
  long long cost4 =
      std::chrono::duration_cast<std::chrono::microseconds>(end - begin)
          .count();

  printf("pthread mutex cost: %lld, mutex cost: %lld\n", cost3, cost4);
  printf("pthread mutex avgcost: %.06f, mutex avgcost: %.06f\n",
         cost3 / float(n), cost4 / float(n));

  return 0;
}
