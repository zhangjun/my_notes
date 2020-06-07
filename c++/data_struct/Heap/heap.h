#pragma once 

template <typename T>
class Heap {
public:
	Heap(int cap): cap_(cap) {
		buffer_ = new T[cap_];
	}

	~Head() {
		if(buffer_ != nullptr) {
			delete [] buffer_;
		}
	}
	
	void push(T t);
	T& pop() const;
private:
	int cap_ = 0;
	int size_ = 0;
	T* buffer_;
}
