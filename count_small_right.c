
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "lib/helpers.h"

void ArrayMergeSortMerge2(int *nums, int num_size, int start, int mid, int end, int *helper, int *counts, int *indices)
{
	int i = 0, j = 0, k = 0, count = 0;
	int *indices_tmp = calloc(num_size, sizeof(int));

	for (i = start; i <= end; ++i) // <=
		helper[i] = nums[i];

	i = start;
	j = mid + 1;
	k = start;

	while ((i <= mid) && (j <= end))
	{
		if (helper[i] <= helper[j])
		{
			counts[indices[i]] += count;
			indices_tmp[k] = indices[i];

			nums[k++] = helper[i++];
		}
		else
		{
			++count;
			indices_tmp[k] = indices[j];

			nums[k++] = helper[j++];
		}
	}

	while (i <= mid)
	{
		counts[indices[i]] += count;
		indices_tmp[k] = indices[i];
	
		nums[k++] = helper[i++];
	}

	while (j <= end)
		indices_tmp[k++] = indices[j++];

//	printf("index (new)org");
	for (i = start; i <= end; ++i)
	{
		indices[i] = indices_tmp[i];
//		printf(" (%d)%d", i, indices[i]);
	}
//	printf("\n");

	free(indices_tmp);
}

void ArrayMergeSortHelper2(int *nums, int num_size, int start, int end, int *helper, int *counts, int *indices)
{
	int mid = ((start + end) >> 1);

	if (start == end)
		return;

	ArrayMergeSortHelper2(nums, num_size, start, mid, helper, counts, indices);
	ArrayMergeSortHelper2(nums, num_size, mid + 1, end, helper, counts, indices);
	ArrayMergeSortMerge2(nums, num_size, start, mid, end, helper, counts, indices);
}

int* ArrayMergeSort2(int *nums, int num_size)
{
	int *helper = calloc(num_size, sizeof(int));
	int *counts = calloc(num_size, sizeof(int));
	int *indices = calloc(num_size, sizeof(int));

	int i = 0;
	for (i = 0; i < num_size; ++i)
		indices[i] = i;

	ArrayMergeSortHelper2(nums, num_size, 0, num_size - 1, helper, counts, indices);

	free(indices);
	free(helper);

	return counts;
}

void DumpArray(int *nums, int num_size)
{
	int i = 0;

	for (i = 0; i < num_size; ++i)
		printf(" %d", nums[i]);
	printf("\n");
}

void Test(int num)
{
/*	int *nums = calloc(num, sizeof(int));
	int i = 0;

	srand(time(NULL));

	for (i = 0; i < num; ++i)
		nums[i] = rand() % 100;
*/
//	int nums[] = {2, 0, 1};
	// [2,0,0]
	int nums[] = {26,78,27,100,33,67,90,23,66,5,38,7,35,23,52,22,83,51,98,69,81,32,78,28,94,13,2,97,3,76,99,51,9,21,84,66,65,36,100,41};
	// [10,27,10,35,12,22,28,8,19,2,12,2,9,6,12,5,17,9,19,12,14,6,12,5,12,3,0,10,0,7,8,4,0,0,4,3,2,0,1,0]
	num = sizeof(nums)/sizeof(nums[0]);

	DumpArray(nums, num);
	
	DumpArray(ArrayMergeSort2(nums, num), num);

//	DumpArray(nums, num);
}

int main(int argc, char *argv[])
{
	Test(/*atoi(argv[1])*/0);

	return 0;
}

