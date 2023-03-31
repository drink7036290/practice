
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <limits.h>
#include <time.h>

int* grayCode(int n, int* returnSize)
{
	int *results = NULL, j = 1;

	*returnSize = (1 << n);

	results = calloc(*returnSize, sizeof(int));

	results[0] = 0;
	results[1] = 1;

	while (j < n)
	{
		int i = 0, val = (1 << j);
		int sum = (val << 1) - 1;

		for (i = val - 1; i >= 0; --i)
			results[sum - i] = results[i] + val;

		++j;
	}

	return results;
}

int main(int argc, char *argv[])
{
	int n = atoi(argv[1]), return_size = 0, i = 0;

	int *results = grayCode(n, &return_size);

	for (i = 0; i < return_size; ++i)
		printf(" %d(0x%x)", results[i], results[i]);
	printf("\n");

	return 0;
}

