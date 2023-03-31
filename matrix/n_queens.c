
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../lib/helpers.h"

int IsValid(int *pos, int i, int j)
{
	int k = 0;

	for (k = 0; k < i; ++k)
	{
		if (pos[k] == j) // same col
			return 0;

		if ((i - k) == abs(j - pos[k])) // diagonal
			return 0;

		// duplicated
//		if ((i - k == 1) && (abs(j - pos[k]) <= 1)) // neighbor
//			return 0;
	}

	return 1;
}

int GetWaysHepler(int *pos, int num, int index)
{
	int j = 0, ret = 0;

	if (index == num)
		return 1;

	for (j = 0; j < num; ++j)
	{
		if (! IsValid(pos, index, j))
			continue;
		
		pos[index] = j; // row index, col j
		
		ret += GetWaysHepler(pos, num, index + 1);
	}

	return ret;
}

int GetWays(int num)
{
	int *pos = calloc(num, sizeof(int));

	return GetWaysHepler(pos, num, 0);
}

int main(int argc, char *argv[])
{
	printf("%d\n", GetWays(atoi(argv[1])));

	// 3 -> 0	
	// 4 -> 2
	// 5 -> 10
	// 6 -> 4
	// 7 -> 40
	// 8 -> 92

	return 0;
}

