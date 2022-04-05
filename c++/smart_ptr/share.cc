#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Task : public std::enable_shared_from_this<Task> {
public:
  Task(const std::string &name) : name_(name) {}
  void AddGroup(std::vector<std::shared_ptr<Task>> &group) {
    group.push_back(shared_from_this());
  }
  ~Task() { std::cout << "~Task: " << name_ << std::endl; }

private:
  std::string name_;
};

int main() {
  std::vector<std::shared_ptr<Task>> group;
  auto task = std::make_shared<Task>("main");
  auto task1 = std::make_shared<Task>("task1");
  task->AddGroup(group);
  task1->AddGroup(group);
  return 0;
}
