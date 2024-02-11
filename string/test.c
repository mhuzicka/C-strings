#include <stdio.h>
#include <stdlib.h>

#include "string_s.h"

int main() {
	String a = NULL, b = NULL, d = NULL, e = NULL;

	//a = strWrite("a");

	//b = strWrite("b");

	strNew(&a);

	strWrite(&d, "ABCDEFGI");

	strAdd(d, d);

	strWrite(&e, "abcdefgh");

	strSubstr(d, 0, 8, &a);

	strSubstr(e, 0, 5, &b);

	strFit(e);
	strFit(d);

	printf("e = %s ; %llu\n", str_c(e), strLength(e));
	printf("a = %s ; %llu\n", str_c(a), strLength(a));

	printf("d = %s ; %llu\n", str_c(d), strLength(d));
	printf("b = %s ; %llu\n", str_c(b), strLength(b));

	a = strDel(a);
	b = strDel(b);
	d = strDel(d);
	strDel(e);
	return 0;
}