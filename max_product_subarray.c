
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "lib/helpers.h"

int MaxProduct(int *nums, int num_size)
{
	int i = 0;

	int accu_max = 1, accu_min = 1;
	int max = INT_MIN;

	for (i = 0; i < num_size; ++i)
	{
		int product_max = accu_max * nums[i];
		int product_min = accu_min * nums[i];

		accu_max = Max(product_min , Max(product_max, nums[i]));
		accu_min = Min(product_min , Min(product_max, nums[i]));

		max = Max(max, accu_max);
	}

	return max;
}

int main(int argc, char *argv[])
{
	int nums[] = {-2, 0, -1};

	printf("%d\n", MaxProduct(nums, sizeof(nums)/sizeof(nums[0])));

	return 0;
}

