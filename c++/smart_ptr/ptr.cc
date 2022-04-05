#include <iostream>
#include <memory>
#include <vector>

int main() {
  std::vector<int> data;
  // std::shared_ptr<std::vector<int>> ptr(&data);
  std::shared_ptr<std::vector<int>> ptr =
      std::make_shared<std::vector<int>>(data);
  // std::shared_ptr<std::vector<int>> ptr =
  // std::make_shared<std::vector<int>>(std::move(data));
  std::cout << ptr.use_count() << std::endl;
  return 0;
}
