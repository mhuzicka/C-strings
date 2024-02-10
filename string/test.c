#include <stdio.h>
#include <stdlib.h>

#include "string_s.h"

int main() {
	String /*a, b,*/ d, e;
	int i;

	//a = strWrite("a");

	//b = strWrite("b");

	d = strWrite("ABCDEFGI");

	e = strWrite("abcdefgh");

	i = strCompareIgnoreCase(e, d);
	printf("%d", i);

	printf("%s\n%llu", str_c(e), strLength(e));

	//strToUpper(e);

	//i = strCut(e, 1, 3);

	//printf("%d", i);

	printf("%s\n%llu", str_c(d), strLength(d));

	//a = strDel(a);
	//b = strDel(b);
	d = strDel(d);
	strDel(e);
	return 0;
}