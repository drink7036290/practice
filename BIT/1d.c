
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../lib/helpers.h"

struct NumArray
{
	int *nums;
	int num_size;

	int *BIT; // binary indexed tree
};

int BITShift(int index)
{
	return (index & (-index));
}

int NumArraySumUntil(struct NumArray *num_array, int index)
{
	int i = index + 1, sum = 0;

	while (i > 0) // >
	{
		sum += num_array->BIT[i];
		i -= BITShift(i);
	}

	return sum;
}

int NumArrayRangeSum(struct NumArray *num_array, int start, int end)
{
	return NumArraySumUntil(num_array, end) - NumArraySumUntil(num_array, start - 1);
}

void NumArrayUpdate(struct NumArray *num_array, int index, int val)
{
	int i = index + 1; // +1

	int diff = val - num_array->nums[index];
	num_array->nums[index] = val;

	while (i <= num_array->num_size) // <=
	{
		num_array->BIT[i] += diff;
		i += BITShift(i);
	}
}

void NumArrayExit(struct NumArray *num_array)
{
	free(num_array->BIT);
}

void NumArrayInit(struct NumArray *num_array, int *nums, int num_size)
{
	int i = 0;

	num_array->nums = nums;
	num_array->num_size = num_size;

	num_array->BIT = calloc(num_size + 1, sizeof(int)); // len + 1

	for (i = 0; i < num_size; ++i)
		num_array->BIT[i + 1] = num_array->BIT[i] + nums[i];

	for (i = num_size; i >= 1; --i)
		num_array->BIT[i] -= num_array->BIT[i - BITShift(i)];
}

void Test(void)
{
	int nums[] = {3, 0, 1, 4};

	struct NumArray num_array;

	NumArrayInit(&num_array, nums, sizeof(nums)/sizeof(nums[0]));

	printf("range sum %d\n", NumArrayRangeSum(&num_array, 1, 3));
	
	NumArrayUpdate(&num_array, 1, 1);

	printf("range sum %d\n", NumArrayRangeSum(&num_array, 1, 3));
}

int main(int argc, char *argv[])
{
	Test();

	return 0;
}

