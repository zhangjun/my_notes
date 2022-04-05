#include <iostream>
#include <queue>
#include <vector>

template <typename T, typename C> class Queue {
public:
  Queue() = default;

private:
  std::priority_queue<T, std::vector<T>, C> queue_;
};

struct cmp {
  bool operator()(const std::pair<int, int> &lhs,
                  const std::pair<int, int> &rhs) {
    return lhs.first > rhs.first;
  }
};

int main() {
  std::vector<int> data{2, 4, 8, 1, 9, 5, 7, 11};
  std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,
                      cmp>
      q;
  for (int i = 0; i < data.size(); i++) {
    q.push(std::pair<int, int>(data[i], i));
  }

  while (!q.empty()) {
    std::cout << q.top().first << "," << q.top().second << std::endl;
    q.pop();
  }
  return 0;
}
