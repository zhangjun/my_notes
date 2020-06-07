#include <iostream>
#include <chrono>
#include <climits>
//#include <x86intrin.h>
#include <immintrin.h>
//#include <avx2intrin.h>

float max_sse(float *a, int n) {
    float res;

    __m128 *f4 = (__m128*) a;
    __m128 maxval = _mm_setzero_ps();

    for (int i = 0; i < n / 4; i++) {
        maxval = _mm_max_ps(maxval, f4[i]);
    }

    for (int i = 0; i < 3; i++) {
        maxval = _mm_max_ps(maxval, _mm_shuffle_ps(maxval, maxval, 0x93));
    }

    _mm_store_ss(&res, maxval);

    return res;
}

int find_max(int *a, int n) {
	int res = INT_MIN;
	for(int i = 0; i < n; i ++) {
		if(a[i] > res) res = a[i];
	}
	return res;
}

int max_sse_int(int *a, int n) {
    int res;

    __m128i *i4 = (__m128i*) a;
    __m128i maxval = _mm_setzero_si128();

    for (int i = 0; i < n / 4; i++) {
        maxval = _mm_max_epi32(maxval, i4[i]);
    }

    for (int i = 0; i < 3; i++) {
        maxval = _mm_max_epi32(maxval, _mm_shuffle_epi32(maxval, _MM_SHUFFLE(2, 1, 0, 3)));
    }

    res = _mm_extract_epi32(maxval, 0);

    return res;
}

int main() {
	__attribute__((aligned(32))) int data[8] = {2, 3, 4, 5, 9, 8, 7, 2346};
	__attribute__((aligned(32))) int data1[8] = {2, 3, 4, 5, 9, 8, 7, 6};
	__attribute__((aligned(16))) int* result = new int[8];
	for(int i = 0; i < 8; i ++) result[i] = 0;
	__m128i a = _mm_load_si128((__m128i*)data);

	
	int N = 1024;
	__attribute__((aligned(32))) int* arr = new int[N*N];
	for(int i = 0; i < N * N; i ++) {
		arr[i] = i;
	}

	auto begin = std::chrono::steady_clock::now();
	int res = max_sse_int(arr,  N * N);
	auto end = std::chrono::steady_clock::now();
	auto time_diff = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	std::cout << "res: " << res << ", cost: " << time_diff << std::endl;
	
	begin = std::chrono::steady_clock::now();
	res = find_max(arr, N * N);
	end = std::chrono::steady_clock::now();
	time_diff = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	std::cout << "res: " << res << ", cost: " << time_diff << std::endl;

	////b = _mm256_broadcastsi128_si256(a);
	//_mm256_storeu_si256((__m256i*)result, c); // not align required
	//for(int i = 0; i < 8; i ++) {
	//	std::cout << b[i] << std::endl;
	//}
	return 0;
}
