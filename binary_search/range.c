
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <limits.h>
#include <time.h>

#include "../lib/helpers.h"

int BinarySearch(int *nums, int num_size, int target, int is_left)
{
	int start = 0, end = num_size - 1;
	int index = -1;

	while (start <= end)
	{
		int mid = ((start + end) >> 1);

		if (nums[mid] == target)
			index = mid;

		if (is_left)
		{
			if (nums[mid] >= target)
				end = mid - 1;
			else
				start = mid + 1;
		}
		else
		{
			if (nums[mid] <= target)
				start = mid + 1;
			else
				end = mid - 1;
		}
	}

	return index;
}

void GetRange(int *nums, int num_size, int target)
{
	int left = BinarySearch(nums, num_size, target, 1);
	int right = BinarySearch(nums, num_size, target, 0);

	printf("%d,%d\n", left, right);
}

int main(int argc, char *argv[])
{
	int nums[] = 
		{5,7,8,8,8,10};
	int target = 6;

	GetRange(nums, sizeof(nums)/sizeof(nums[0]), target);

	return 0;
}

