#include <iostream>
#include <immintrin.h>

int main() {
	__attribute__((aligned(32))) int data[8] = {2, 3, 4, 5, 9, 8, 7, 6};

	__m256i value = _mm256_load_si256((__m256i*)data);
	
	int shlf_mask = _MM_SHUFFLE(2, 3, 0, 1);
	std::cout << "mask: " << shlf_mask << std::endl;

	// 2, 3, 4, 5 -> 3, 2, 5, 4 
	__m256i ss = _mm256_shuffle_epi32(value, _MM_SHUFFLE(2, 3, 0, 1)); // 3, 2, 1, 0 -> 2, 3, 0, 1
	int a = _mm256_extract_epi32(ss, 0);
	int b = _mm256_extract_epi32(ss, 1);
	int c = _mm256_extract_epi32(ss, 2);
	int d = _mm256_extract_epi32(ss, 3);
	std::cout << a << ", " << b << ", " << c << ", " << d << std::endl;

	a = _mm256_extract_epi32(ss, 4);
	b = _mm256_extract_epi32(ss, 5);
	c = _mm256_extract_epi32(ss, 6);
	d = _mm256_extract_epi32(ss, 7);
	std::cout << a << ", " << b << ", " << c << ", " << d << std::endl;
	return 0;
}
