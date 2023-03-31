
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "../lib/helpers.h"

long long gRecursiveCount = 0;

int LIS(int *nums, int num_size, int index, int min, int *mem_with)
{
	int len_with = 0, len_without = 0;

	++gRecursiveCount;

	if (index >= num_size)
		return 0;

	// with
	if (nums[index] > min)
	{
		if (mem_with[index] == 0)
			mem_with[index] = 1 + LIS(nums, num_size, index + 1, nums[index], mem_with);

		len_with = mem_with[index];
	}

	// without
	len_without = LIS(nums, num_size, index + 1, min, mem_with);
//	printf("index %d len with %d without %d\n", index, len_with, len_without);

	return Max(len_with, len_without);
}

int LongestIncreasingSubsequence(int *nums, int num_size)
{
	int *mem_with = calloc(num_size, sizeof(int));

	int len = LIS(nums, num_size, 0, INT_MIN, mem_with);

	free(mem_with);

	return len;
}

int N2(int *nums, int num_size)
{
	int i = 0, len = 0;
	int *lens = calloc(num_size, sizeof(int));

	for (i = 0; i < num_size; ++i)
	{
		int j = 0, val = nums[i];

		lens[i] = 1;

		for (j = 0; j < i; ++j)
		{
			if (nums[j] < val)
				lens[i] = Max(lens[i], lens[j] + 1);
		}	
	}

	for (i = 0; i < num_size; ++i)
		len = Max(len, lens[i]);

	free(lens);

	return len;
}

int LeftMostOrClosestIndex(int *temp, int start, int end, int target)
{
	int index = INT_MAX, index_left = -1;

	while (start <= end)
	{
		int mid = (start + end) / 2;

		if (temp[mid] == target)
			index = Min(index, mid);	
	
		if (temp[mid] >= target)
			end = mid - 1;
		else
		{
			index_left = mid;
			start = mid + 1;
		}
	}

	if (index == INT_MAX)
		index = index_left;

	return index;
}

int Nlogn(int *nums, int num_size)
{
	int i = 0, len_temp = 1;
        int *temp = calloc(num_size, sizeof(int));

	temp[0] = nums[0];

        for (i = 1; i < num_size; ++i)
        {
		int j = 0;
		int val = nums[i];

		int index = LeftMostOrClosestIndex(temp, 0, len_temp - 1, val);
		// -1 ~ len_temp - 1
//		printf("got %d len_temp %d index %d\n", val, len_temp, index);

		if (index == -1)
			temp[index + 1] = val;
		else if (temp[index] < val)
		{
			temp[index + 1] = val;

			if (index == len_temp - 1)
				++len_temp;
		}

		for (j = 0; j < len_temp; ++j)
			printf(" %d", temp[j]);
		printf("\n");
	}

        free(temp);

        return len_temp;
}

int main(int argc, char *argv[])
{
#define kNumSize 10
//	int nums[kNumSize] = {10,9,2,5,3,7,101,18};
	int nums[kNumSize] = {67,78,57,52,99,86,80,26,12,6};

//#define kNumSize 6
//	int nums[kNumSize] = {0,1,0,3,2,3};

//#define kNumSize 7
//	int nums[kNumSize] = {7,7,7,7,7,7,7};

	printf("%d\n", LongestIncreasingSubsequence(nums, kNumSize));
	printf("recursive count %lld\n", gRecursiveCount);

	printf("N2 %d\n", N2(nums, kNumSize));

	printf("Nlogn %d\n", Nlogn(nums, kNumSize));
	
	return 0;
}

