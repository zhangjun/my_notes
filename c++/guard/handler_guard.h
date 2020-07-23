#pragma once 

namespace common {
class HandlerGuard {
public:
    HandlerGuard(Hander h):cb_(h), execute_(true) {}
    ~HandlerGuard() {
        if(execute_) {
            cb_();
        }
    }
    void set_execute(bool v) {
        execute_ = v;
    }
private:
    Handler cb_;
    bool execute_;
};
}
