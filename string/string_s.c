/* --- STRING LIBRARY FOR C language --- */

/* --- THIS VERSION IS FOR MSVC COMPILER --- */

#define __STDC_WANT_LIB_EXT1__ 1

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "string_s.h"


// if length of string is lesser than this value, size of
// string is this value
#define MINIMAL_SIZE 16

#define GROWTH_FACTOR 2

typedef unsigned long long uint64;

typedef struct string {
	char* text;
	uint64 length;
	uint64 size;
}*String;


#define TOLOWER(C) ( ((C) >= 65 && (C) <= 90) ? (C) + 32 : (C) )
#define TOUPPER(C) ( ((C) >= 97 && (C) <= 122) ? (C) - 32 : (C) )


// -- MEMORY MANAGEMENT --

String __cdecl strDel(String str) {
	if (str != NULL) {
		if (str->text != NULL) {
			free(str->text);
		}

		free(str);
	}

	return NULL;
}

static String __cdecl strAlloc(uint64 length) {
	String out;
	uint64 size = (length < MINIMAL_SIZE) ?
		MINIMAL_SIZE * sizeof(char) : length * GROWTH_FACTOR * sizeof(char);

	if ((out = (String)malloc(sizeof(struct string))) == NULL ||
		(out->text = (char*)malloc(size)) == NULL) {

		if (out != NULL) {
			free(out);
		}

		return NULL;
	}

	out->size = size;
	return out;
}

static void __cdecl strRealloc(String str, const uint64 new_length) {
	assert(str->size < new_length);

	uint64 new_size = new_length * GROWTH_FACTOR * sizeof(char);
	
	char* temp = (char*)realloc(str->text, new_size);

	if (temp) {
		str->size = new_size;
		str->text = temp;
	}
}

void __cdecl strFit(String str) {
	assert(str);

	char* temp = (char*)realloc(str->text, str->length * sizeof(char));

	if (temp) {
		str->text = temp;
		str->size = str->length * sizeof(char);
	}
}


// -- GETTERS --

uint64 __cdecl strLength(const String str) {
	assert(str);

	return str->length - 1;
}

const char* __cdecl str_c(const String str) {
	assert(str);

	return str->text;
}


// -- TEXT MANIPULATION --

void __cdecl strNew(String* pstr) {
	if (*pstr) {
		*pstr = strDel(*pstr);
	}

	*pstr = strAlloc(1);

	if (*pstr) {
		(*pstr)->text[0] = '\0';
		(*pstr)->length = 1;
	}
}

void __cdecl strWrite(String* pstr, const char* str_c) {
	uint64 allocLength;

	if (*pstr) {
		*pstr = strDel(*pstr);
	}

	if (str_c == NULL || str_c[0] == '\0') {
		strNew(pstr);
	}
	else {
		allocLength = strlen(str_c) + 1;

		if ((*pstr = strAlloc(allocLength)) == NULL) {
			return;
		}

		(*pstr)->length = allocLength;

		if (strcpy_s((*pstr)->text, (*pstr)->size, str_c)) {
			*pstr = strDel(*pstr);
		}
	}
}

String __cdecl strConcat(const String str1, const String str2) {
	assert(str1 && str2);

	uint64 numOfChars = str1->length + str2->length - 1;
	String out;

	assert(numOfChars < (0xFF'FFFF'FFFF)); // absurdly long ( > 1 TB)

	if ((out = strAlloc(numOfChars)) == NULL) {
		return NULL;
	}

	if (strcpy_s(out->text, out->size, str1->text) ||
		strcpy_s(out->text + str1->length - 1, out->size, str2->text) ) {
		return strDel(out);
	}

	out->length = numOfChars;

	return out;
}

errno_t __cdecl strAdd(String destination, const String source) {
	assert(destination);

	if (source == NULL || source->length < 2) {
		return 1;
	}

	uint64 numOfChars = destination->length + source->length - 1;

	assert(numOfChars < (0xFF'FFFF'FFFF)); // absurdly long ( > 1 TB)

	if (numOfChars > destination->size) {
		strRealloc(destination, numOfChars);
		if (destination->size < numOfChars) {
			return 2;
		}
	}

	if ( memcpy_s(destination->text + destination->length - 1,
		destination->size - destination->length - 1,
		source->text, source->length) ) {
		return 3;
	}

	destination->length = numOfChars;

	return 0;
}

void __cdecl strToLower(String str) {
	uint64 i = 0;

	assert(str);

	while (i < str->length) {
		(str->text)[i] = TOLOWER((str->text)[i]);

		i++;
	}
}

void __cdecl strToUpper(String str) {
	uint64 i = 0;

	assert(str);

	while (i < str->length) {
		(str->text)[i] = TOUPPER((str->text)[i]);

		i++;
	}
}

void __cdecl strSubstr(const String str, unsigned long long start, unsigned long long end, String* pout) {
	uint64 length;
	char* temp;

	assert(str);
	assert(start < str->length);
	assert(end < str->length);

	if (*pout) {
		*pout = strDel(*pout);
	}

	if (start > end) {  }
	else if (start == end) {
		strWrite(pout, "\0");
	}
	else {
		length = end - start + 1; // terminating char

		if ((*pout = strAlloc(length)) == NULL) {
			return;
		}

		temp = str->text + start;
		if (memcpy_s((*pout)->text, length, temp, length)) {
			*pout = strDel(*pout);
			return;
		}

		(*pout)->text[length - 1] = '\0';
		(*pout)->length = length;
	}
}


// -- TEXT INFORMATION GETTERS --

int __cdecl strCompare(const String str1, const String str2) {
	assert(str1 && str2);

	return strcmp(str1->text, str2->text);
}

int __cdecl strCompareFast(const String str1, const String str2) {
	uint64 index = 0;

	assert(str1 && str2);

	if (str1->length != str2->length) {
		return 1;
	}

	while (index != str1->length) {
		if ((str1->text)[index] != (str2->text)[index]) {
			return 1;
		}

		index++;
	}

	return 0;
}

int __cdecl strCompareIgnoreCase(const String str1, const String str2) {
	uint64 index = 0;

	assert(str1 && str2);

	if (str1->length != str2->length) {
		return 1;
	}

	while (index != str1->length) {
		if (TOLOWER((str1->text)[index]) != TOLOWER((str2->text)[index])) {
			return 1;
		}

		index++;
	}

	return 0;
}

inline char __cdecl strAt(const String str, unsigned long long index) {
	assert(str);

	return index > str->length ? -1 : str->text[index];
}

char* __cdecl strCharArray(const String str) {
	char* out;

	assert(str);

	if ((out = (char*)malloc(str->length * sizeof(char))) == NULL) {
		return NULL;
	}

	if (strcpy_s(out, str->length, str->text) != 0) {
		return NULL;
	}

	return out;
}

uint64 __cdecl strSum(const String str) {
	uint64 index = 0;
	uint64 sum = 0;

	assert(str);

	while (index != str->length) {
		sum += (str->text)[index];
		index++;
	}

	return sum;
}


// -- TODO --

/*
short strNumber(const String str, double* value, const short mode) {
	uint64 index = 0;
	char decimal = 0;

	while (index != str->length) {
		if ((str->text)[index] == 46) {
			if (decimal != 0) {
				return 1;
			}

			decimal = index;
		}
		else if ((str->text)[index] < 48 || (str->text)[index] > 57) {
			return 1;
		}
		else {
			// TODO
		}

		index++;
	}

	return 0;
}
*/
