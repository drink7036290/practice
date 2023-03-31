
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "lib/helpers.h"


int FindEndOfLeft(int *nums, int num_size)
{
	int i = 0;

	for (i = 1; i < num_size; ++i)
	{
		if (nums[i - 1] > nums[i])
			break;
	}

	return i - 1;
}

int FindStartOfRight(int *nums, int num_size)
{
	int i = 0;

	for (i = num_size - 2; i >= 0; --i)
	{
		if (nums[i] > nums[i + 1])
			break;
	}

	return i + 1;
}

int BinarySearch(int *nums, int start, int end, int target)
{
	int last_index = -1;

	while (start <= end)
	{
		int mid = ((start + end) >> 1);

		if (nums[mid] == target)
			return mid;

		last_index = mid;

		if (nums[mid] > target)
			end = mid - 1;
		else
			start = mid + 1;
	}

	return last_index;
}

struct Range
{
	int start;
	int end; // included
};

void RangeDump(struct Range *range)
{
	printf("start %d end %d\n", range->start, range->end);
}

void RangeInit(struct Range *range, int start, int end)
{
	range->start = start;
	range->end = end;
}

struct Range* FindRange(int *nums, int num_size)
{
	int end_of_left = -1, start_of_right = -1;
	int min = INT_MAX, max = INT_MAX;
	int i = 0;

	struct Range *range = calloc(1, sizeof(struct Range));

	if ((end_of_left = FindEndOfLeft(nums, num_size)) == num_size - 1)
		return NULL;
	if ((start_of_right = FindStartOfRight(nums, num_size)) == 0)
		return NULL;

	min = nums[start_of_right];
	max = nums[end_of_left];
	for (i = end_of_left; i <= start_of_right; ++i)
	{
		min = Min(min, nums[i]);
		max = Max(max, nums[i]);
	}

	RangeInit(range
			, BinarySearch(nums, 0, end_of_left, min)
			, BinarySearch(nums, start_of_right, num_size - 1, max));

	RangeDump(range);

	return range;
}


int main(int argc, char *argv[])
{
	int nums[] = {1, 2, 4, 7, 10, 11, 7, 12, 6, 7, 16, 18, 19};

	FindRange(nums, sizeof(nums)/sizeof(nums[0]));

	return 0;
}

