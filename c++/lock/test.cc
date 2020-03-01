#include "rwlock.h"

RWLock mtx;

void func() {
	for(int i = 0; i < 1000; i ++) {
		std::lock_guard<RWLock> lck(mtx);
	}
}

int main() {
	std::thread f1(func);
	std::thread f2(func);

	f1.join();
	f2.join();
	return 0;
}
