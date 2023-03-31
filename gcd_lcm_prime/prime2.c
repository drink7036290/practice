
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../lib/helpers.h"

int TestBit(int *flags, int val)
{
	int index = (val >> 5);
	int shift = (val & 0x1F);

	return (flags[index] & (1 << shift));
}

void SetBit(int *flags, int val)
{
	int index = (val >> 5);
	int shift = (val & 0x1F);

	flags[index] |= (1 << shift);
}

void PrintPrime(int max_num)
{
	// flag 1 : non prime
	int *flags = calloc((max_num >> 5) + 1, sizeof(int));

	int i = 0, val = 2;

	for (val = 2; val <= max_num; ++val)
	{
		if (TestBit(flags, val))
			continue;

		for (i = val * val; i <= max_num; i += val)
			SetBit(flags, i);
	}

	for (val = 2; val <= max_num; ++val)
	{
		if (TestBit(flags, val))
			continue;

		printf(" %d\n", val);
	}
}

int main(int argc, char *argv[])
{
	PrintPrime(atoi(argv[1]));

	return 0;
}

