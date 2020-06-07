#include <iostream>
#include "variant_coding.h"

int main() {
	char buffer[64] = {0};
	
	int num = 2345335;
	int result = 0;
	encode_varint(buffer, num);
	std::cout << "buffer: " << std::endl;
	
	std::cout << "result: " << decode_varint(buffer, buffer + sizeof(buffer) - 1) << std::endl;
	return 0;
}
