
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

int GetMajority(int *nums, int num_size)
{
	int i = 0, target = -1;
	int count = 0;

	for (i = 0; i < num_size; ++i)
	{
		if (count == 0)
		{
			target = nums[i];
			++count;

			continue;
		}

		if (nums[i] == target)
			++count;
		else
			--count;
	}

	count = 0;
	for (i = 0; i < num_size; ++i)
	{
		if (nums[i] == target)
			++count;
	}

	return (count > (num_size >> 1)) ? target :  -1;
}

int main(int argc, char *argv[])
{
	int nums[] = {3, 1, 7, 1, 1, 7, 7, 3, 7, 7, 7};

	printf("%d\n", GetMajority(nums, sizeof(nums)/sizeof(nums[0])));

	return 0;
}

