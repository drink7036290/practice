
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <limits.h>
#include <time.h>

#include "lib/helpers.h"

void Helper(int *nums, int num_size, int start)
{
	int i = 0;

	if (start == num_size - 1)
	{
		for (i = 0; i < num_size; ++i)
			printf(" %d", nums[i]);
		printf("\n");

		return;
	}

	Helper(nums, num_size, start + 1);

	for (i = start + 1; i < num_size; ++i) // + 1
	{
		Swap(&nums[start], &nums[i]);
		Helper(nums, num_size, start + 1);
		Swap(&nums[i], &nums[start]); // restore
	}
}

int main(int argc, char *argv[])
{
	int nums[] = {1,2,3,4};

	Helper(nums, sizeof(nums)/sizeof(nums[0]), 0);

	return 0;
}

