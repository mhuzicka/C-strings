/* --- STRING LIBRARY FOR C language --- */

/* --- THIS VERSION IS FOR MSVC COMPILER --- */

/* GitHub: mhuzicka
* 
* This library contains functions that are often used when
* working with arrays of characters (or strings). The String
* data structure provides safer methods for working with
* strings in C by taking care of memory management when
* working with strings, although it is not as optimal as
* using strings in C++.
* 
* Some operations can still cause a memory leak, but every
* such case - and how to evade it - is explained in the
* comments for each function.
* 
* The library is useful for smaller projects where performance
* is not that big of an issue.
*/

#pragma once

/* A safer method for storing and operating with an array of
chars. */
typedef struct string* String;

/* Returns the length of the string without terminating
character. */
unsigned long long __cdecl strLength(const String str);

/*Returns pointer to chars in the string.*/
const char* __cdecl str_c(const String str);

/* Creates an empty string or returns NULL if memory could
not be allocated. */
void __cdecl strNew(String* pstr);

/* Creates a string containing given sequence of characters. */
void __cdecl strWrite(String* pstr, const char* str_c);

/* Deallocates the string. Always returns NULL. */
String __cdecl strDel(String str);

/* Concatenates strings.

WARNING: creates a new string, so a MEMORY LEAK will happen
if the return value is either of inputs. Use strAdd for a
memory-safe method. */
String __cdecl strConcat(const String str1, const String str2);

/* Concatenates str2 to str1. Destination must be a valid
String. Returns a non-zero value if str1 was not modified. */
errno_t __cdecl strAdd(String destination, const String source);

/* Compares two strings. Same as strcmp() from string.h. */
int __cdecl strCompare(const String str1, const String str2);

/* Compares two strings, but only returns 1 if not equal and
0 if equal. */
int __cdecl strCompareFast(const String str1, const String str2);

/* Compares two strings. Same as strCompareFast, but ignores
case. */
int __cdecl strCompareIgnoreCase(const String str1, const String str2);

/* Returns character at given index. Returns -1 if string is
shorter than index. */
char __cdecl strAt(const String str, unsigned long long index);

/* Creates an array of chars from a String (creates a
duplicate). */
char* __cdecl strCharArray(const String str);

/* Returns sum of ASCII values of characters in the String. */
unsigned long long __cdecl strSum(const String str);

/* TODO
If String can be converted to a number, returns 1.
Otherwise returns 0 and does not modify variable value.

If there are more than one number located in the String,
function will read number defined by this precedence:
1 - the first hexadecimal number
2 - the first binary number
3 - the first decimal number

The */
short __cdecl strNumber(String str, double* value, short mode);

/* TODO
Checks if the String contains a given substring. The return
value represents how many substrings does the String
contain. */
int __cdecl strContains(const String str, const String substring);

/* TODO
Checks if the String contains a given substring. Returns 1
if yes and 0 if not. Faster than strContains.*/
int __cdecl strContainsFast(const String str, const String substring);

/* Creates a string from a substring in str.

If start == end, creates an empty string.

If start > end, returns NULL. */
void __cdecl strSubstr(const String str, unsigned long long start, unsigned long long end, String* pout);

/* TODO
Splits a String into multiple Strings. Returns an array of
Strings. Output_size is a number of strings in an array.*/
String* __cdecl strSplit(const String str, char splitter, unsigned long long* output_size);

/* To lowercase. */
void __cdecl strToLower(String str);

/* To uppercase. */
void __cdecl strToUpper(String str);

/* Deallocation of redundant allocated memory. */
void __cdecl strFit(String str);