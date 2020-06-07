#include <iostream>
//#include <x86intrin.h>
#include <immintrin.h>
//#include <avx2intrin.h>

int main() {
	int data[4] = {2, 3, 4, 5};
	int* result = new int[8];
	for(int i = 0; i < 8; i ++) result[i] = 0;
	__m128i a = _mm_load_si128((__m128i*)data);
	__m256i b;

	b = _mm256_broadcastsi128_si256(a);
	std::cout << _mm256_extract_epi32(b, 0) << std::endl;
	std::cout << _mm256_extract_epi32(b, 1) << std::endl;
	std::cout << _mm256_extract_epi32(b, 2) << std::endl;
	std::cout << _mm256_extract_epi32(b, 3) << std::endl;

	std::cout << _mm256_extract_epi32(b, 4) << std::endl;
	std::cout << _mm256_extract_epi32(b, 5) << std::endl;
	std::cout << _mm256_extract_epi32(b, 6) << std::endl;
	std::cout << _mm256_extract_epi32(b, 7) << std::endl;

	__m128i l;
	l = _mm256_extractf128_si256(b, 0);
	{
		std::cout << _mm_extract_epi32(l, 0) << " ";
		std::cout << _mm_extract_epi32(l, 1) << " ";
		std::cout << _mm_extract_epi32(l, 2) << " ";
		std::cout << _mm_extract_epi32(l, 3) << " ";
		std::cout << std::endl;

		l = _mm256_extractf128_si256(b, 1);
		std::cout << _mm_extract_epi32(l, 0) << " ";
		std::cout << _mm_extract_epi32(l, 1) << " ";
		std::cout << _mm_extract_epi32(l, 2) << " ";
		std::cout << _mm_extract_epi32(l, 3) << " ";
		std::cout << std::endl;
	}

	l = _mm256_castsi256_si128(b);
	std::cout << _mm_extract_epi32(l, 0) << std::endl;
	std::cout << _mm_extract_epi32(l, 1) << std::endl;
	std::cout << _mm_extract_epi32(l, 2) << std::endl;
	std::cout << _mm_extract_epi32(l, 3) << std::endl;
	return 0;
}
