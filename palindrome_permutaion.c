
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "lib/helpers.h"

void Toggle(int *bits, int pos)
{
	int mask = 1 << pos;

	if (*bits & mask)
		*bits &= ~mask;
	else
		*bits |= mask;
}

int IsPalindromePermutaion(char *str)
{
	int bits = 0;

	char *c = str;

	while (*c != '\0')
	{
		Toggle(&bits, *c - 'A');

		++c;
	}

	return ((bits & (bits - 1)) == 0);
}

int main(int argc, char *argv[])
{
	char str[] = "abcXb"/*"TactXCoa"*/;

	printf("%d\n", IsPalindromePermutaion(str));

	return 0;
}

