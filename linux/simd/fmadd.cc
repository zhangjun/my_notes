#include <iostream>
#include <immintrin.h>

void fmadd(int* a, int* b, int* c, int* d, int N) {
}

__m256 mul_addv(__m256 a, __m256 b, __m256 c) {
    return _mm256_add_ps(_mm256_mul_ps(a, b), c);
}

int main() {
	__attribute__((aligned(32))) float a[8] = {2, 3, 4, 5, 9, 8, 7, 6};
	__attribute__((aligned(32))) float b[8] = {2, 3, 4, 5, 9, 8, 7, 6};
	__attribute__((aligned(32))) float c[8] = {2, 3, 4, 5, 9, 8, 7, 6};

	__attribute__((aligned(32))) float r[8] = {2, 3, 4, 5, 9, 8, 7, 6};

	__m256 value_a = _mm256_load_ps(a);
	__m256 value_b = _mm256_load_ps(b);
	__m256 value_c = _mm256_load_ps(c);
	__m256 value_r = _mm256_load_ps(r);

	value_r = _mm256_fmadd_ps(value_a, value_b, value_c);

	std::cout << value_r[0] << std::endl;

	__m128 low = _mm256_extractf128_ps(value_r, 0);
	__m128 high = _mm256_extractf128_ps(value_r, 1);

	std::cout << low[0] << std::endl;
	
	float r0 = _mm_extract_ps(low, 0);
	float r1 = _mm_extract_ps(low, 1);
	float r2 = _mm_extract_ps(low, 2);
	float r3 = _mm_extract_ps(low, 3);
	std::cout << (int)r0 << ", " << r1 << ", " << r2 << ", " << r3 << std::endl;

	//a = _mm256_extract_epi32(ss, 4);
	//b = _mm256_extract_epi32(ss, 5);
	//c = _mm256_extract_epi32(ss, 6);
	//d = _mm256_extract_epi32(ss, 7);
	//std::cout << a << ", " << b << ", " << c << ", " << d << std::endl;
	return 0;
}
