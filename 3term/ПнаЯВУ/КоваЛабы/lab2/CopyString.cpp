#include "StringCopy.h"

void StringCopy(char* s1, const char* s2, int length) {
	for (int i = 0; i < length; i++) {
		s1[i] = s2[i];
	}
	s1[length] = '\0';
}

