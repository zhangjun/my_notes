#include <cstdio>
#include <cstdlib>
#include "String.h"

int main() {
	char* data = (char*)malloc(48);
	const char* content = "this is an test.";
	memcpy(data, content, strlen(content));
	printf("%s\n", data);

	String* s = new String(content);
	printf("%s\n", s -> data_);
	return 0;
}
