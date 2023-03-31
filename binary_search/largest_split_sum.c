
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "../lib/helpers.h"

int TotalSum(int *nums, int num_size)
{
	int i = 0, sum = 0;

	for (i = 0; i < num_size; ++i)
		sum += nums[i];

	return sum;
}

int SingleMax(int *nums, int num_size)
{
	int i = 0, max = INT_MIN;

	for (i = 0; i < num_size; ++i)
		max = Max(max, nums[i]);

	return max;
}

int GetSplitNum(int *nums, int num_size, int sum_max)
{
	int i = 0, sum = 0, m = 1; // [TRICKEY] no cut means m = 1

	for (i = 0; i < num_size; ++i)
	{
		if (sum + nums[i] > sum_max)
		{
			++m;
			sum = 0;
		}

		sum += nums[i];
	}

	return m;
}

int LargestSplitSum(int *nums, int num_size, int m)
{
	int min = SingleMax(nums, num_size);
	int max = TotalSum(nums, num_size);

	int sum = INT_MAX;

	while (min <= max) // <=
	{
		int mid = (min + max) / 2;

		int split = GetSplitNum(nums, num_size, mid);

		printf("min %d mid %d max %d split %d / org sum %d\n", min, mid, max, split, sum);

		if (split == m)
			sum = Min(sum, mid); // Don't break here, continue mid - 1

		if (split > m)
			min = mid + 1;
		else
			max = mid - 1;
	}

	if (sum == INT_MAX)
		sum = min;

	return sum;
}

int main(int argc, char *argv[])
{
//#define kNumSize 5
//	int nums[kNumSize] = {7,2,5,10,8};
//	int m = 2;

//#define kNumSize 5
//	int nums[kNumSize] = {1,2,3,4,5};
//	int m = 2;

#define kNumSize 3
	int nums[kNumSize] = {1, 4, 4};
	int m = 3;

//#define kNumSize 7
//	int nums[kNumSize] = {2,3,1,1,1,1,1};
//	int m = 5;

	printf("%d\n", LargestSplitSum(nums, kNumSize, m));

	return 0;
}

