
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../lib/helpers.h"

int MaxMinsHelper(int *nums, int num_size, int index, int *memo)
{
	int ret = 0;

	if (index >= num_size)
		return 0;

	if (memo[index] > 0)
		return memo[index];

	// with
	ret = nums[index] + MaxMinsHelper(nums, num_size, index + 2, memo);

	// without
	ret = Max(ret, MaxMinsHelper(nums, num_size, index + 1, memo));

	memo[index] = ret;

	return ret;
}

int MaxMins(int *nums, int num_size)
{
	int *memo = calloc(num_size, sizeof(int));

	return MaxMinsHelper(nums, num_size, 0, memo);
}

int MaxMinsOptimal(int *nums, int num_size)
{
	int i = 0;
	
	int curr = 0, curr_plus1 = 0, curr_plus2 = 0;

	for (i = num_size - 1; i >= 0; --i)
	{
		curr = Max(nums[i] + curr_plus2, curr_plus1);

		curr_plus2 = curr_plus1;
		curr_plus1 = curr;
	}

	return curr;
}

int main(int argc, char *argv[])
{
	int nums[] = {30, 15, 60, 75, 45, 15, 15, 45};

	printf("%d %d\n", MaxMins(nums, sizeof(nums)/sizeof(nums[0]))
		, MaxMinsOptimal(nums, sizeof(nums)/sizeof(nums[0]))
	);

	return 0;
}

