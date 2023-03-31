
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <limits.h>
#include <time.h>

#include "../lib/helpers.h"

int CheckPosition(int r1, int c1, int r2, int c2)
{
	if (c1 == c2)
		return 0;

	if (abs(r1 - r2) == abs(c1 - c2))
		return 0;

	return 1;
}

int NQueensHelper(int n, int row, int *cols)
{
	int r = 0, c = 0, cn = 0;

	if (row == n)
		return 1;

	for (c = 0; c < n; ++c)
	{
		int check = 1;

		for (r = 0; r < row; ++r)
		{
			if (! CheckPosition(row, c, r, cols[r]))
			{
				check = 0;
				break;
			}
		}

		if (! check)
			continue;

		cols[row] = c;
		cn += NQueensHelper(n, row + 1, cols);
	}

	return cn;
}

int NQueens(int n)
{
	int *cols = calloc(n, sizeof(int));

	return NQueensHelper(n, 0, cols);
}

int main(int argc, char *argv[])
{
	printf("%d\n", NQueens(atoi(argv[1])));
	return 0;
}

