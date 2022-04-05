#include <chrono>
#include <iostream>
#include <x86intrin.h>

void add(const float *a, const float *b, float *c, int N) {
  for (int i = 0; i < N; i++) {
    c[i] = a[i] + b[i];
  }
}

void vec_add(const float *a, const float *b, float *c, int N) {
  __m128 i0, i1, i2;
  int i;
  for (i = 0; (i + 4) <= N; i += 4) {
    i0 = _mm_load_ps(a + i);
    i1 = _mm_load_ps(b + i);
    i2 = _mm_add_ps(i0, i1);
    _mm_store_ps(c + i, i2);
  }
  add(a + i, b + i, c + i, N - i);
}

void test() {
  float data[4] = {1.0, 2.0, 3.0, 4.0};
  __m128 m = _mm_load_ps(data);

  float *result = new float[4];
  _mm_store_ps(result, m);
  _mm_store1_ps(result, m);
  _mm_storer_ps(result, m);
  for (int i = 0; i < 4; i++) {
    std::cout << result[i] << std::endl;
  }
}

int main() {
  int N = 256 * 256;
  float *a, *b, *c;
  a = new float[N], b = new float[N], c = new float[N];
  for (int i = 0; i < N; i++) {
    srand(time(NULL));
    int n = rand() * 10 / 1.23;
    a[i] = n;
    n = rand() * 10 / 0.71;
    b[i] = n;
    a[i] = n;
  }
  auto begin = std::chrono::steady_clock::now();
  vec_add(a, b, c, N);
  auto end = std::chrono::steady_clock::now();
  auto time_diff =
      std::chrono::duration_cast<std::chrono::microseconds>(end - begin)
          .count();
  std::cout << "cost: " << time_diff << std::endl;
  return 0;
}
