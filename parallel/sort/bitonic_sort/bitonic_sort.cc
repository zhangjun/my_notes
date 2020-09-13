#include <iostream>
#include <vector>

template <typename T>
class Compare {
public:
	bool operator()(const T& lsh, const T& rsh) {
		return lsh > rsh;
	}
};

template <typename T>
void swap(T& lsh, T& rsh) {
	T tmp = rsh;
	rsh = lsh;
	lsh = tmp;
}

template <typename T, class Cmp = Compare<T>, bool asc = true>
void Merge(T data[], int begin, int count) {
	if(count > 1) {
		int stride = count / 2;
		for(int i = begin; i < begin + stride; i ++) {
			// asc  i > i + stride, !asc i < i + stride
			if(asc == (data[i] > data[i + stride]))	swap<T>(data[i], data[i + stride]);
		}
		Merge<T, Compare<T>, asc>(data, begin, stride);
		Merge<T, Compare<T>, asc>(data, begin + stride, stride);
	}
} 

template <typename T, class Cmp = Compare<T>, bool asc = true>
void BitonicSort(T data[], int begin, int count) {
	if(count > 1) {
		int stride = count / 2;
		BitonicSort<T, Compare<T>, true>(data, begin, stride);
		BitonicSort<T, Compare<T>, false>(data, begin + stride, stride);
		Merge<T, Compare<T>, asc>(data, begin, count);
	}
}

int main() {
	std::vector<int> data{3, 1, 7, 4, 3, 9, 10, 5}; // size: 2^n
	BitonicSort<int, Compare<int>>(data.data(), 0, data.size());
	for(auto &it : data)
		std::cout << it << std::endl;
	return 0;
}
