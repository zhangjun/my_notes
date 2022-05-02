#include "thread_pool.h"

#include <atomic>
#include <functional>
#include <queue>
#include <stdexcept>
#include <utility>
#include <vector>
#include <iostream>

namespace threadpool {
API_DECL_VISIBILITY ThreadPool::ThreadPool(uint32_t size) : is_stoped_(false) {
  idle_thrd_num_ = size < 1 ? 1 : size;

  for (uint32_t i = 0; i < idle_thrd_num_; ++i) {
    pool_.emplace_back(ThreadFunc, this);
  }
}

API_DECL_VISIBILITY ThreadPool::~ThreadPool() {
  is_stoped_.store(true);
  {
    std::unique_lock<std::mutex> lock{m_lock_};
    cond_var_.notify_all();
  }

  for (std::thread &thd : pool_) {
    if (thd.joinable()) {
      try {
        thd.join();
      } catch (const std::system_error &) {
        std::cerr << "system error" << std::endl;
      } catch (...) {
        std::cerr << "exception" << std::endl;
      }
    }
  }
}

void ThreadPool::ThreadFunc(ThreadPool *thread_pool) {
  if (thread_pool == nullptr) {
    return;
  }
  while (!thread_pool->is_stoped_) {
    std::function<void()> task;
    {
      std::unique_lock<std::mutex> lock{thread_pool->m_lock_};
      thread_pool->cond_var_.wait(
        lock, [thread_pool] { return thread_pool->is_stoped_.load() || !thread_pool->tasks_.empty(); });
      if (thread_pool->is_stoped_ && thread_pool->tasks_.empty()) {
        return;
      }
      task = std::move(thread_pool->tasks_.front());
      thread_pool->tasks_.pop();
    }
    --thread_pool->idle_thrd_num_;
    task();
    ++thread_pool->idle_thrd_num_;
  }
}
}  // namespace thradpool