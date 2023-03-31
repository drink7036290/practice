
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <limits.h>
#include <time.h>

void InsertionSort(int *nums, int num_size)
{
	int i = 0;

	for (i = 0; i < num_size; ++i)
	{
		int tmp = nums[i];

		int j = i - 1;
		while ((j >= 0) && (nums[j] > tmp))
		{
			nums[j + 1] = nums[j];
			--j;
		}

		nums[j + 1] = tmp;
	}
}

void DumpArray(int *nums, int num_size)
{
	int i = 0;

//	printf("DumpArray\n");
	for (i = 0; i < num_size; ++i)
		printf(" %d", nums[i]);
	printf("\n");
}

void Test(int num_size)
{
	int *nums = calloc(num_size, sizeof(int));
	int i = 0;

	srand(time(NULL));
	
	for (i = 0; i < num_size; ++i)
		nums[i] = rand() % 100;

	DumpArray(nums, num_size);

	InsertionSort(nums, num_size);

	DumpArray(nums, num_size);

	free(nums);
}

int main(int argc, char *argv[])
{
	Test(atoi(argv[1]));

	return 0;
}

