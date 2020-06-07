#include "heap.h"


template <typename T>
T& Heap<T>::pop() const {
}

template <typename T>
T& Heap<T>::pop() const {
	buffer_[0] = buffer_[-- size_];
	sift_down(0);
}
