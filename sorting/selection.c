
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <limits.h>
#include <time.h>

#include "../lib/helpers.h"

void SelectionSort(int *nums, int num_size)
{
	int i = 0, j = 0;

	for (i = 0; i < num_size; ++i)
	{
		int min = nums[i];
		int index = i;

		for (j = i + 1; j < num_size; ++j)
		{
			if (min > nums[j])
			{
				min = nums[j];
				index = j;
			}
		}

		if (index != i)
			Swap(&nums[index], &nums[i]);
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

	SelectionSort(nums, num_size);

	DumpArray(nums, num_size);

	free(nums);
}

int main(int argc, char *argv[])
{
	Test(atoi(argv[1]));

	return 0;
}

