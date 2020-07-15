#include <mutex>
#include <condition_variable>

class Semaphore {
public:
    Semaphore(int value): count_(value), wakeups{0} {}

    void wait() {
        std::unique_lock<std::mutex> lck{mutex_};
        if(-- count < 0) {
            cv_.wait(lck, [&]() -> bool { return wakeups > 0;});
            -- wakeups;
        }
    }

    void signal() {
        std::unique_lock<std::mutex> lck{mutex_};
        if(++ count <= 0) {
            ++ wakeups;
            cv_.notify_one();
        }
    }

private:
    int count_;
    int wakeups_;
    std::mutex mutex_;
    std::condition_variable cv_;
};
