
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <limits.h>
#include <time.h>

int missing(int *nums, int num_size)
{
	int i = 0, ret = 0;

	for (i = 0; i < num_size; ++i)
	{
		ret ^= i;
		ret ^= nums[i];
	}

	return (ret ^ num_size);
}

int main(int argc, char *argv[])
{
	int nums[] =
//		{0, 1, 3};
		{2, 0, 3};

	printf("%d\n", missing(nums, sizeof(nums)/sizeof(nums[0])));

	return 0;
}

