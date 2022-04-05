#include <chrono>
#include <iostream>
//#include <x86intrin.h>
#include <immintrin.h>
//#include <avx2intrin.h>

int sum(int *data) {
  __m128i value = _mm_load_si128((__m128i *)data);
  __m128i result;
  result = _mm_hadd_epi32(value, value);
  result = _mm_hadd_epi32(result, result);
  // int a = _mm_cvtsi128_si32(result);
  // int b = _mm_cvtsi128_si32(_mm_shuffle_epi32(result,0x55));
  // int c = _mm_cvtsi128_si32(_mm_shuffle_epi32(result,0xAA));
  // int d = _mm_cvtsi128_si32(_mm_shuffle_epi32(result,0xFF));
  // std::cout << a << "," << b << "," << c << "," << d << std::endl;
  // int ret = _mm_cvtsi128_si32(result);
  int ret = _mm_extract_epi32(result, 0);
  return ret;
}

// sse add
int sum_v1(int *data, int n) {
  __m128i *p = (__m128i *)data;

  int ret = 0;
  __m128i s0 = _mm_setzero_si128();
  __m128i s1 = _mm_setzero_si128();
  __m128i s2 = _mm_setzero_si128();
  __m128i s3 = _mm_setzero_si128();

  __m128i l0, l1, l2, l3;

  int block = n / 16, reserve = n % 16;
  for (int i = 0; i < block; i++) {
    l0 = _mm_load_si128(p);
    l1 = _mm_load_si128(p + 1);
    l2 = _mm_load_si128(p + 2);
    l3 = _mm_load_si128(p + 3);
    s0 = _mm_add_epi32(s0, l0);
    s1 = _mm_add_epi32(s1, l1);
    s2 = _mm_add_epi32(s2, l2);
    s3 = _mm_add_epi32(s3, l3);
    p += 4;
  }

  int *v = (int *)p;
  for (int i = 0; i < reserve; i++) {
    ret += v[i];
  }

  // sum reduce
  s0 = _mm_add_epi32(s0, s1);
  s2 = _mm_add_epi32(s2, s3);

  s0 = _mm_add_epi32(s0, s2);

  // m128i sum
  s0 = _mm_hadd_epi32(s0, s0);
  s0 = _mm_hadd_epi32(s0, s0);

  ret += _mm_extract_epi32(s0, 0); // low 32 bit
  return ret;
}

// avx add
int sum_v2(int *data, int n) {
  __m256i *p = (__m256i *)data;

  int ret = 0;
  __m256i s0 = _mm256_setzero_si256();
  __m256i s1 = _mm256_setzero_si256();
  __m256i s2 = _mm256_setzero_si256();
  __m256i s3 = _mm256_setzero_si256();

  __m256i l0, l1, l2, l3;

  int block = n / 32, reserve = n % 32;
  for (int i = 0; i < block; i++) {
    l0 = _mm256_loadu_si256(p);
    l1 = _mm256_loadu_si256(p + 1);
    l2 = _mm256_loadu_si256(p + 2);
    l3 = _mm256_loadu_si256(p + 3);
    s0 = _mm256_add_epi32(s0, l0);
    s1 = _mm256_add_epi32(s1, l1);
    s2 = _mm256_add_epi32(s2, l2);
    s3 = _mm256_add_epi32(s3, l3);
    p += 4;
  }

  int *v = (int *)p;
  for (int i = 0; i < reserve; i++) {
    ret += v[i];
  }

  // sum reduce
  s0 = _mm256_add_epi32(s0, s1);
  s2 = _mm256_add_epi32(s2, s3);

  s0 = _mm256_add_epi32(s0, s2);

  __m128i r = _mm_add_epi32(_mm256_castsi256_si128(s0),
                            _mm256_extracti128_si256(s0, 1));

  // m128i sum
  r = _mm_hadd_epi32(r, r);
  r = _mm_hadd_epi32(r, r);

  ret = _mm_extract_epi32(r, 0); // low 32 bit
  return ret;
}

int main() {
  __attribute__((aligned(32))) int data[8] = {2, 3, 4, 5, 9, 8, 7, 6};
  __attribute__((aligned(32))) int data1[8] = {2, 3, 4, 5, 9, 8, 7, 6};
  __attribute__((aligned(16))) int *result = new int[8];

  int ret = sum(data);
  std::cout << "sum # ret: " << ret << std::endl;

  const int N = 1024 * 1024;
  __attribute__((aligned(32))) int *arr = new int[N];
  for (int i = 0; i < N; i++)
    arr[i] = i % 10;

  auto begin = std::chrono::steady_clock::now();
  ret = sum_v1(arr, N);
  auto end = std::chrono::steady_clock::now();
  auto time_diff =
      std::chrono::duration_cast<std::chrono::microseconds>(end - begin)
          .count();
  std::cout << "sum_v1 # ret: " << ret << ", cost: " << time_diff << std::endl;

  begin = std::chrono::steady_clock::now();
  ret = sum_v2(arr, N);
  end = std::chrono::steady_clock::now();
  time_diff = std::chrono::duration_cast<std::chrono::microseconds>(end - begin)
                  .count();
  std::cout << "sum_v2 # ret: " << ret << ", cost: " << time_diff << std::endl;
  return 0;
}
