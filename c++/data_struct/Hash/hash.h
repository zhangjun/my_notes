#pragma once 

template <typename T>
class Hash {
public:
	Hash(int cap): cap_(cap) { data_.resize(cap_); for(int i = 0; i < data_.size(); i ++) data_[i] = -1; }
	void set(const T& t);
	T get(int key) const;
	int search(int key);
private:
	std::vector<T> data_;
	int cap_;
};

int Hash::search(int key) {
	int addr = key % cap_;
	while(i < cap_ && data_[addr].key != -1 && data[addr].key != key) {
		i ++;
		addr = (addr + 1) % cap_;
	}
	if(i == cap_) return -1;
	else return addr;
}

void Hash::set(const T& t) {
	int addr = search(t.key);
	if(addr ==  -1 || data[addr].key == t.key) {
		return 1;
	} else {
		data_[addr] = t;
		return 0;
	}
}

T Hash::get(int key) const {
	
}

