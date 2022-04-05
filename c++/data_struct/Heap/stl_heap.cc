#include <algorithm>
#include <iostream>
#include <vector>

// --std=c++1z
void print_vector(const std::vector<int> &data) {
  std::for_each(data.begin(), data.end(),
                [](std::remove_reference_t<decltype(data)>::value_type item) {
                  std::cout << item << " ";
                });
  std::cout << std::endl;
}

int main() {
  std::vector<int> data{7, 2, 8, 2, 1, 5, 3, 9, 4};
  std::make_heap(data.begin(), data.end(), std::less<int>());
  std::cout << "make_heap: " << std::endl;
  print_vector(data);

  data.push_back(11);
  std::push_heap(data.begin(), data.end(), std::less<int>());
  print_vector(data);

  std::cout << "pop_heap:" << std::endl;
  std::pop_heap(data.begin(), data.end(), std::less<int>());
  data.pop_back();
  print_vector(data);
  std::cout << std::endl;
  return 0;
}
