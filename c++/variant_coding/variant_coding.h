#pragma once 

char* encode_varint(char* buf, uint64_t val) {
	uint8_t* ptr = reinterpret_cast<uint8_t*>(buf);
	while(val > 127) {
		*(ptr ++) = val | 0x80;	
		val >>= 7;
	}
	*(ptr ++) = static_cast<uint8_t>(val);
	return reinterpret_cast<char*>(ptr);
}

uint64_t decode_varint(const char* buf, const char* limit) {
	uint64_t result = 0, byte;
	for(uint32_t shift = 0; shift <= 63 && buf < limit; shift += 7) {
		byte = *(reinterpret_cast<const uint8_t*>(buf ++));
		result |= ((byte & 0x7F) << shift);
		if((byte & 0x80) == 0)
			break;
	}
	return result;
	
}
