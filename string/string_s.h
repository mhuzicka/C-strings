/* --- STRING LIBRARY FOR C language --- */

/* --- THIS VERSION IS FOR MSVC COMPILER --- */

/*GitHub: mhuzicka
* 
* This library contains functions that are often used when
* working with arrays of characters (or strings). The String
* data structure provides safer methods for working with
* strings in C by taking care of memory management when
* working with strings, although it is not as optimal as
* using strings in C plus plus.
* 
* Some operations can still cause a memory leak, but every
* such case - and how to evade it - is explained in the
* comments for each function.
* 
* Although value for length of a string is unsigned int,
* the length should be less than the value of INT_MAX
* macro, though some operations theoretically support
* Strings with length of the max value the unsigned integer
* can support.
* 
* The library is useful for smaller projects or where
* performance is not that big of an issue. */

#pragma once

/*A safer method for storing and operating with an array of
chars.*/
typedef struct string* String;

/*Returns the length of the string without terminating
character.*/
unsigned long long strLength(const String str);

/*Returns pointer to chars in the string.*/
const char* str_c(const String str);

/*Returns a string containing given sequence of characters.*/
String strWrite(const char* str);

/* Deallocates the string. Should be called for every call
of strwrite(). Always returns NULL.*/
String strDel(String str);

/*Concatenates str2 to str1 and return a String containing
character sequence of str1 followed by character sequence
of str2. Returns NULL if a block of memory could not be
allocated.

WARNING: allocates a new String, so the return value should
not be assigned to str1 or str2 and should always be
assigned to a variable, otherwise causes a MEMORY LEAK! Use
strAdd() to overwrite str1.*/
String strConcat(const String str1, const String str2);

/*Concatenates str2 to str1. Unlike strconcat(), strAdd()
cannot cause a memory leak, but it modifies str1. Does
nothing if a block of memory could not be allocated.
Returns a non-zero value if str1 was not modified.*/
errno_t strAdd(String destination, const String source);

/*Compares two strings.

Returns 0 if strings are identical.
Returns value of the first unique char from the first
string minus the value of char from second string at the
same location.*/
int strCompare(const String str1, const String str2);

/*Compares two strings, but only returns 1 if not equal and
0 if equal.*/
int strCompareFast(const String str1, const String str2);

/*Compares two strings. Same as strCompareFast, but ignores
case.*/
int strCompareIgnoreCase(const String str1, const String str2);

/*Returns character at given index. Returns -1 if string is
shorter than index.*/
inline char strAt(const String str, unsigned long long index);

/*Creates an array of chars from a String (duplicates the
array of chars).*/
char* strCharArray(const String str);

/*Returns sum of ASCII values of characters in the String.*/
unsigned long long strSum(const String str);

/*If String can be converted to a number, returns 1.
Otherwise returns 0 and does not modify variable value.

If there are more than one number located in the String,
function will read number defined by this precedence:
1 - the first hexadecimal number
2 - the first binary number
3 - the first decimal number

The */
short strNumber(String str, double* value, short mode);

/* TODO
Checks if the String contains a given substring. The return
value represents how many substrings does the String
contain.*/
int strContains(const String str, const String substring);

/* TODO
Checks if the String contains a given substring. Returns 1
if yes and 0 if not. Faster than strContains.*/
int strContainsFast(const String str, const String substring);

/*Cuts a String. The str String will contain characters
between a start index (including) and an end index
(excluding). Returns 0 if successful, 1 if the str variable
was not modified.

If start == end, str will contain only terminating char.*/
short strCut(String str, unsigned long long start, unsigned long long end);

/* TODO
Splits a String into multiple Strings. Returns an array of
Strings. Counter contains a number of strings in an array.*/
String* strSplit(String str, char splitter, int* counter);

/*To lowercase.*/
String strToLower(const String str);

/*To uppercase.*/
String strToUpper(const String str);