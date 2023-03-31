
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void counting_sort(int *nums, int num_size, int max)
{
	int *counts = calloc(max + 1, sizeof(int));
	int i = 0, j = 0;

	for (i = 0; i < num_size; ++i)
		++counts[nums[i]];

	i = num_size - 1;

	for (j = max; j >= 0; --j)
	{
		int cn = counts[j];

		while (cn-- > 0)
			nums[i--] = j;
	}

	for (i = 0; i < num_size; ++i)
		printf(" %d", nums[i]);
	printf("\n");
}

int main(int argc, char *argv[])
{
#define kNumSize 8
	int nums[kNumSize] = {2, 5, 3, 0, 2, 3, 0, 3};
	int max = 5;

	counting_sort(nums, kNumSize, max);

	return 0;
}

