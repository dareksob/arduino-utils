#ifndef __DSOBZCAK_UTILS__
#define __DSOBZCAK_UTILS__

#include <Arduino.h>

char* replacement(char* value, const char from, const char to) {
	int i = 0;
	while (value[i] != '\0') {
		if (value[i] == from) {
			value[i] = to;
		}
		i++;
	}

	return value;
};


char* toChar(String value) {
	int length = value.length() + 1;
	char buffer[length];
	value.toCharArray(buffer, length);
	return buffer;
}

#endif