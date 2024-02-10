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

String strDel(String str) {
	if (str != NULL) {
		if (str->text != NULL) {
			free(str->text);
		}

		free(str);
	}

	return NULL;
}

static inline String strRealloc(String str, const uint64 new_length) {
	assert(str->size < new_length);
	
	String temp = (String)realloc(str->text, new_length * GROWTH_FACTOR);

	if (temp) {
		str->size = new_length * GROWTH_FACTOR;
	}
	else {
		strDel(str);
		str->size = 0;
	}

	return temp;
}

void strFit(String* str) {
	String temp = realloc(*str, (*str)->length);

	assert(temp != NULL);

	if (temp) {
		str = &temp;
		(*str)->size = (*str)->length;
	}
}


// -- GETTERS --

uint64 strLength(const String str) {
	return str->length - 1;
}

const char* str_c(const String str) {
	return str->text;
}


// -- TEXT MANIPULATION --

String strWrite(const char* str_c) {
	String out = NULL;

	if ((out = (String)malloc(sizeof(struct string))) == NULL) {
		return NULL;
	}

	if (str_c == NULL || str_c[0] == '\0') {
		out->size = MINIMAL_SIZE;

		if ((out->text = (char*) malloc(out->size * sizeof(char))) == NULL) {
			free(out);
			return NULL;
		}

		out->length = 1;
		out->text[0] = '\0';
		return out;
	}

	out->length = strlen(str_c) + 1;
	out->size = (out->length <= MINIMAL_SIZE) ? MINIMAL_SIZE : out->length * GROWTH_FACTOR;

	if ((out->text = (char*) malloc(out->size * sizeof(char))) == NULL) {
		free(out);
		return NULL;
	}

	if (strcpy_s(out->text, out->length, str_c)) {
		return strDel(out);
	}

	return out;
}

String strConcat(const String str1, const String str2) {
	uint64 numOfChars = str1->length + str2->length - 1;
	String out;

	assert(numOfChars < (0xFF'FFFF'FFFF)); // absurdly long ( > 1 TB)

	if ((out = (String) malloc(sizeof(struct string))) == NULL ||
		(out->text = (char*) malloc(numOfChars * GROWTH_FACTOR * sizeof(char))) == NULL) {

		if (out != NULL) {
			free(out);
		}

		return NULL;
	}

	if (strcpy_s(out->text, str1->length, str1->text) ||
		strcpy_s(out->text + str1->length - 1, str2->length, str2->text) ) {
		return strDel(out);
	}

	out->length = numOfChars;
	out->size = numOfChars * GROWTH_FACTOR;

	return out;
}

errno_t strAdd(String destination, const String source) {
	uint64 numOfChars = destination->length + source->length - 1;
	String temp;

	assert(numOfChars < (0xFF'FFFF'FFFF)); // absurdly long ( > 1 TB)

	if (numOfChars > destination->size) {
		if ((temp = strRealloc(destination, numOfChars)) == NULL) {
			return 1;
		}
		else {
			destination = temp;
		}
	}

	if (strcpy_s(destination->text + destination->length - 1, source->length, source->text)) {
		return 2;
	}

	destination->length = numOfChars;
	destination->size = numOfChars * GROWTH_FACTOR;

	return 0;
}

String strToLower(const String str) {
	uint64 i = 0;
	String out = NULL;

	if ((out = (String)malloc(sizeof(struct string))) == NULL ||
		(out->text = (char*)malloc(str->size * sizeof(char))) == NULL) {
		if (out != NULL) {
			free(out);
		}

		return NULL;
	}

	while (i < str->length) {
		(out->text)[i] = TOLOWER((str->text)[i]);

		i++;
	}

	return out;
}

String strToUpper(const String str) {
	uint64 i = 0;
	String out = NULL;

	if ((out = (String)malloc(sizeof(struct string))) == NULL ||
		(out->text = (char*)malloc(str->size * sizeof(char))) == NULL) {
		if (out != NULL) {
			free(out);
		}

		return NULL;
	}

	while (i < str->length) {
		(out->text)[i] = TOUPPER((str->text)[i]);

		i++;
	}

	return out;
}


// -- TEXT INFORMATION GETTERS --

int strCompare(const String str1, const String str2) {
	return strcmp(str1->text, str2->text);
}

int strCompareFast(const String str1, const String str2) {
	uint64 index = 0;

	if (str1->length != str2->length) {
		return 1;
	}

	while (index != str1->length) {
		if ((str1->text)[index] != (str2->text)[index])
			return 1;

		index++;
	}

	return 0;
}

int strCompareIgnoreCase(const String str1, const String str2) {
	uint64 index = 0;

	if (str1->length != str2->length) {
		return 1;
	}

	while (index != str1->length) {
		if (TOLOWER((str1->text)[index]) != TOLOWER((str2->text)[index]))
			return 1;

		index++;
	}

	return 0;
}

inline char strAt(const String str, unsigned long long index) {
	return index > str->length ? -1 : str->text[index];
}

char* strCharArray(const String str) {
	char* out;

	if ((out = (char*)malloc(str->length * sizeof(char))) == NULL) {
		return NULL;
	}

	if (strcpy_s(out, str->length, str->text) != 0) {
		return NULL;
	}

	return out;
}

uint64 strSum(const String str) {
	uint64 index = 0;
	uint64 sum = 0;

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

short strCut(String str, unsigned long long start, unsigned long long end) {
	uint64 num, size;
	char* new, *temp;

	if (start > end) {
		num = start;
		start = end;
		end = start;
	}

	if (start == end) {
		str->text = realloc(str->text, sizeof(char));
		(str->text)[0] = '\0';
	}
	else if (str->length <= start) {
		return 1;
	}
	else {
		size = end - start;
		if ((new = (char*)malloc((size + 1) * sizeof(char))) == NULL) {
			return 1;
		}

		temp = str->text + start;
		memcpy_s(new, size, temp, size);
		new[size] = '\0';

		free(str->text);
		str->text = new;
		str->length = size + 1;
	}

	return 0;
}
*/