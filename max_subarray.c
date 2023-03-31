
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "lib/helpers.h"

int MaxSubarray(int *nums, int num_size)
{
	int i = 0;
	int accu = 0, max = INT_MIN;
	int start = 0, end = 0;

	for (i = 0; i < num_size; ++i)
	{
		int sum = accu + nums[i];

		if (nums[i] >= sum)
		{
			accu = nums[i];
			start = i;
		}
		else
			accu = sum;

		if (accu > max)
		{
			end = i;
			max = accu;
		}
		
//		accu = Max(accu + nums[i], nums[i]);
//		max = Max(max, accu);
	}

	printf("%d (%d, %d)\n", max, start, end);

	return max;
}

int main(int argc, char *argv[])
{
//	int nums[] = {-2,1,-3,4,-1,2,1,-5,4};
	int nums[] = {1};

	MaxSubarray(nums, sizeof(nums)/sizeof(nums[0]));

	return 0;
}

