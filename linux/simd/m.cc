#include <immintrin.h>
#include <iostream>

// _MM_SHUFFLE(z, y, x, w)   ==   (z << 6) | (y << 4) | (x << 2) | w.
uint32_t hsum_epi32_avx(__m128i x) {
  // high 64bit
  __m128i hi64 =
      _mm_unpackhi_epi64(x, x); // 3-operand non-destructive AVX lets us save a
                                // byte without needing a movdqa
  __m128i sum64 = _mm_add_epi32(hi64, x);
  __m128i hi32 = _mm_shuffle_epi32(
      sum64, _MM_SHUFFLE(2, 3, 0, 1)); // Swap the low two elements
  __m128i sum32 = _mm_add_epi32(sum64, hi32);
  // return _mm_cvtsi128_si32(sum32);       // movd
  return _mm_extract_epi32(sum32, 0); // movd
}

uint32_t hsum_8x32(__m256i v) {
  __m128i sum128 = _mm_add_epi32(
      _mm256_castsi256_si128(v),
      _mm256_extracti128_si256(
          v, 1)); // silly GCC uses a longer AXV512VL instruction :/
  return hsum_epi32_avx(sum128);
}

int main() {
  __attribute__((aligned(32))) int data[8] = {2, 3, 4, 5, 9, 8, 7, 6};

  __m128i value = _mm_load_si128((__m128i *)data);

  uint32_t s = hsum_epi32_avx(value);
  std::cout << "res: " << s << std::endl;

  int mask = _MM_SHUFFLE(2, 3, 0, 1);
  std::cout << "mask: " << mask << std::endl;

  // 2, 3, 4, 5 -> 3, 2, 5, 4
  __m128i ss = _mm_shuffle_epi32(
      value, _MM_SHUFFLE(2, 3, 0, 1)); // 3, 2, 1, 0 -> 2, 3, 0, 1
  int a = _mm_extract_epi32(ss, 0);
  int b = _mm_extract_epi32(ss, 1);
  int c = _mm_extract_epi32(ss, 2);
  int d = _mm_extract_epi32(ss, 3);
  std::cout << a << ", " << b << ", " << c << ", " << d << std::endl;

  __m256i m = _mm256_load_si256((__m256i *)data);
  __m128i h = _mm256_extracti128_si256(m, 1);
  int v = _mm_extract_epi32(h, 0);
  int vv = _mm256_extract_epi32(m, 4);
  std::cout << "v: " << v << ", vv: " << vv << std::endl;
  return 0;
}
