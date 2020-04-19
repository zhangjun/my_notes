#include <iostream>
#include <climits>       // limits.h INT_MAX
//#include <x86intrin.h>
#include <immintrin.h>
//#include <avx2intrin.h>

int sum(int* data) {
	__m128i value = _mm_load_si128((__m128i*)data);
	__m128i result;
	result = _mm_hadd_epi32(value, value);
	result = _mm_hadd_epi32(result, result);
	//int a = _mm_cvtsi128_si32(result);
	//int b = _mm_cvtsi128_si32(_mm_shuffle_epi32(result,0x55));
	//int c = _mm_cvtsi128_si32(_mm_shuffle_epi32(result,0xAA));
	//int d = _mm_cvtsi128_si32(_mm_shuffle_epi32(result,0xFF));
	//std::cout << a << "," << b << "," << c << "," << d << std::endl;
	//int ret = _mm_cvtsi128_si32(result);
	int ret = _mm_extract_epi32(result, 0);
	return ret;
}

int max_v1(int* data, int n) {
	__m128i* p = (__m128i*)data;
	
	int ret = INT_MIN;
	//__m128i s0 = _mm_setzero_si128();
	//__m128i s1 = _mm_setzero_si128();
	//__m128i s2 = _mm_setzero_si128();
	//__m128i s3 = _mm_setzero_si128();
	__m128i s0 = _mm_set_epi32(INT_MIN, INT_MIN, INT_MIN, INT_MIN);
	__m128i s1 = _mm_set_epi32(INT_MIN, INT_MIN, INT_MIN, INT_MIN);
	__m128i s2 = _mm_set_epi32(INT_MIN, INT_MIN, INT_MIN, INT_MIN);
	__m128i s3 = _mm_set_epi32(INT_MIN, INT_MIN, INT_MIN, INT_MIN);

	__m128i l0, l1, l2, l3;

	int block = n / 16, reserve = n % 16;
	for(int i = 0; i < block; i ++) {
		l0 = _mm_load_si128(p);
		l1 = _mm_load_si128(p + 1);
		l2 = _mm_load_si128(p + 2);
		l3 = _mm_load_si128(p + 3);
		s0 = _mm_max_epi32(s0, l0);
		s1 = _mm_max_epi32(s1, l1);
		s2 = _mm_max_epi32(s2, l2);
		s3 = _mm_max_epi32(s3, l3);
		p += 4;
	}

	int* v = (int*)p;
	for(int i = 0; i < reserve; i ++) {
		if(v[i] > ret)
			ret = v[i];
	}
	
	// max reduce
	s0 = _mm_max_epi32(s0, s1);
	s2 = _mm_max_epi32(s2, s3);

	s0 = _mm_max_epi32(s0, s2);

	// m128i sum
	s0 = _mm_hadd_epi32(s0, s0);
	s0 = _mm_hadd_epi32(s0, s0);

	ret = _mm_extract_epi32(s0, 0); // low 32 bit
	return ret;
}

int main() {
	__attribute__((aligned(32))) int data[8] = {2, 3, 4, 5, 9, 8, 7, 6};
	__attribute__((aligned(32))) int data1[8] = {2, 3, 4, 5, 9, 8, 7, 6};
	__attribute__((aligned(16))) int* result = new int[8];

	int ret = sum(data);
	std::cout << "sum # ret: " << ret << std::endl;

	const int N = 1024;
	__attribute__((aligned(16))) int* arr = new int[N];
	for(int i = 0; i < N; i ++) 
		arr[i] = i;

	ret = max_v1(arr, N);
	std::cout << "sum_v1 # ret: " << ret << std::endl;

	return 0;
}
