#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
 public:
  using Task = std::function<void()>;
  explicit ThreadPool(int num_threads): running_(true) {
    threads_.resize(num_threads);
    for (auto& thread : threads_) {
      thread.reset(new std::thread(&ThreadPool::TaskLoop, this));
    }
  }
  ~ThreadPool() {
    {
      std::unique_lock<std::mutex> lock(mutex_);
      running_ = false;
    }
    scheduled_.notify_all();
    for (auto& thread : threads_) {
      thread->join();
      thread.reset(nullptr);
    }
  }
  static ThreadPool* GetInstance() {
    std::call_once(init_flag_, &ThreadPool::Init);
    return threadpool_.get();
  }
  

  ThreadPool(const ThreadPool& pool) = delete;
  ThreadPool& operator=(const ThreadPool& pool) = delete;
  template<class F, class... Args>
  auto Commit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
    if (!running_) {
      throw std::runtime_error("ThreadPool is not running");  
    }
    using RetType = decltype(f(args...));
    auto task = std::make_shared<std::packaged_task<RetType()>>(
      std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
    std::future<RetType> future = task -> get_future();
    {
        std::lock_guard<std::mutex> lock(mutex_);
        tasks_.emplace([task]() {
            (*task)();
        });
    }
    scheduled_.notify_one();
    return std::move(future);
  }

 private:
  void TaskLoop() {
    while (true) {
      Task task;
      {
        std::unique_lock<std::mutex> lock(mutex_);
        scheduled_.wait(
            lock, [this] { return !this->tasks_.empty() || !this->running_; });
        if (!running_ && tasks_.empty()) {
          return;
        }
        task = std::move(tasks_.front());
        tasks_.pop();
      }
      task();
    }
  }
  static void Init() {
    if (threadpool_ == nullptr) {
      int num_threads = std::thread::hardware_concurrency();
      threadpool_.reset(new ThreadPool(num_threads));
    }
  }

 private:
  static std::unique_ptr<ThreadPool> threadpool_;
  static std::once_flag init_flag_;

  std::vector<std::unique_ptr<std::thread>> threads_;
  std::queue<Task> tasks_;
  std::mutex mutex_;
  bool running_;
  std::condition_variable scheduled_;
};

std::unique_ptr<ThreadPool> ThreadPool::threadpool_ = nullptr;
std::once_flag ThreadPool::init_flag_;