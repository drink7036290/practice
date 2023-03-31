
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <limits.h>
#include <time.h>

#include "../lib/helpers.h"

void BubbleSort(int *nums, int num_size)
{
	int i = 0, j = 0;

	for (j = 0; j < num_size; ++j)
	{
		for (i = num_size - 1; i > j; --i)
		{
			if (nums[i - 1] > nums[i])
				Swap(&nums[i - 1], &nums[i]);
		}
	}
}

void DumpArray(int *nums, int num_size)
{
	int i = 0;

//	printf("DumpArray\n");
	for (i = 0; i < num_size; ++i)
		printf(" %d", nums[i]);
	printf("\n");
}

void Test(int num_size)
{
	int *nums = calloc(num_size, sizeof(int));
	int i = 0;

	srand(time(NULL));
	
	for (i = 0; i < num_size; ++i)
		nums[i] = rand() % 100;

	DumpArray(nums, num_size);

	BubbleSort(nums, num_size);

	DumpArray(nums, num_size);

	free(nums);
}

int main(int argc, char *argv[])
{
	Test(atoi(argv[1]));

	return 0;
}

