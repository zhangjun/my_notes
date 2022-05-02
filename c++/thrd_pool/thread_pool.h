#pragma once
#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <queue>
#include <stdexcept>
#include <thread>
#include <utility>
#include <vector>

#if defined(_WIN32)
#define API_DECL_VISIBILITY __declspec(dllexport)
#else
#define API_DECL_VISIBILITY __attribute__((visibility("default")))
#endif  // _WIN32

namespace threadpool {
using ThreadTask = std::function<void()>;

class API_DECL_VISIBILITY ThreadPool {
 public:
  explicit ThreadPool(uint32_t size = 4);
  ~ThreadPool();

  template <class Func, class... Args>
  auto commit(Func &&func, Args &&... args) -> std::future<decltype(func(args...))> {
    using retType = decltype(func(args...));
    std::future<retType> fail_future;
    if (is_stoped_.load()) {
      return fail_future;
    }

    auto bindFunc = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
    auto task = std::make_shared<std::packaged_task<retType()>>(bindFunc);
    if (task == nullptr) {
      return fail_future;
    }
    std::future<retType> future = task->get_future();
    {
      std::lock_guard<std::mutex> lock{m_lock_};
      tasks_.emplace([task]() { (*task)(); });
    }
    cond_var_.notify_one();
    return future;
  }

  static void ThreadFunc(ThreadPool *thread_pool);

 private:
  std::vector<std::thread> pool_;
  std::queue<ThreadTask> tasks_;
  std::mutex m_lock_;
  std::condition_variable cond_var_;
  std::atomic<bool> is_stoped_;
  std::atomic<uint32_t> idle_thrd_num_;
};
}  // namespace threadpool