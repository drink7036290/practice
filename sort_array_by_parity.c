
#include "stdio.h"
#include "stdlib.h"

#define kStrLen 1

void PrintArray(int *nums, int len)
{
	int i = 0;

	for (i = 0; i < len; ++i)
		printf (" %d", nums[i]);

	printf("\n");
}

void Swap(int *nums, int index1, int index2)
{
	int tmp = nums[index1];
	nums[index1] = nums[index2];
	nums[index2] = tmp;
}

void SortArrayByParity(int *nums, int len)
{
	int i = 0, j = len - 1;

	if (len <= 0)
		return;

	while (i < j)
	{
		if ((nums[i] % 2 == 0) || (nums[j] % 2 != 0))
			continue;

		Swap(nums, i, j);

		++i;
		--j;
	}
}

int main(int argc, char *argv[])
{
	int nums[kStrLen] = {0};

	PrintArray(nums, kStrLen);

	SortArrayByParity(nums, kStrLen);

	PrintArray(nums, kStrLen);

	return 0;
}

