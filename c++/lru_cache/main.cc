#include "LRUCache.h"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

struct Bitmap {
  void *data;
  int font_size;
  int len;
};

class ValueDeleter {
public:
  void operator()(const Bitmap &value) const {
    if (value.data != nullptr) {
      free(value.data);
      // value.data = nullptr;
    }
  }
};

typedef LRUHandle<std::string, Bitmap> handle;

class BitmapCache {
public:
  const handle *get(const std::string &key);
  const handle *put(const std::string &key, const Bitmap &bitmap);
  const size_t size() const { return cache_.size(); }

private:
  LRUCache<std::string, Bitmap, ValueDeleter> cache_;
};

const handle *BitmapCache::get(const std::string &key) {
  return cache_.get(key);
}

const handle *BitmapCache::put(const std::string &key, const Bitmap &bitmap) {
  return cache_.put(key, bitmap);
}

int main() {
  BitmapCache *cache = new BitmapCache();
  const handle *h;
  for (int i = 0; i < 1000; i++) {
    Bitmap *b = (Bitmap *)malloc(sizeof(Bitmap));
    b->font_size = 10;
    b->len = 10240;
    std::chrono::steady_clock::time_point begin =
        std::chrono::steady_clock::now();
    b->data = malloc(b->len);
    std::chrono::steady_clock::time_point mid =
        std::chrono::steady_clock::now();
    std::string key = "key" + std::to_string(i);
    h = cache->put(key, *b);
    std::chrono::steady_clock::time_point end =
        std::chrono::steady_clock::now();
    std::cout << "cost: ["
              << std::chrono::duration_cast<std::chrono::microseconds>(mid -
                                                                       begin)
                     .count()
              << ","
              << std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                       mid)
                     .count()
              << "]" << std::endl;
  }

  std::vector<std::vector<int>> data(100, std::vector<int>(100));
  std::chrono::steady_clock::time_point b = std::chrono::steady_clock::now();
  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 100; j++) {
      data[i][j] = j;
    }
  }
  std::chrono::steady_clock::time_point e = std::chrono::steady_clock::now();
  std::cout
      << "t: "
      << std::chrono::duration_cast<std::chrono::microseconds>(e - b).count()
      << std::endl;

  std::string key = "key" + std::to_string(100);
  std::chrono::steady_clock::time_point begin =
      std::chrono::steady_clock::now();
  h = cache->get(key);
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::cout << cache->size() << std::endl;
  std::cout << "cost: "
            << std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                     begin)
                   .count()
            << std::endl;
  return 0;
}
