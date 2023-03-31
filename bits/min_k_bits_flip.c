
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../lib/helpers.h"

int minKBitFlips(int* nums, int num_size, int k)
{
	int i = 0, curr_flip = 0, num_flip = 0;

	for (i = 0; i < num_size; ++i)
	{
//		printf("i %d num %d curr %d\n", i, nums[i], curr_flip);

		if (i >= k)
		{
			curr_flip ^= (nums[i - k] > 1); // 2 or 3
//			printf("reset curr %d / i-k %d num %d\n", curr_flip, i-k, nums[i - k]);
		}

		if (curr_flip == nums[i])
		{
			if (i + k > num_size) // i + (k - 1) <= num_size - 1
			{
//				printf("i+k %d size %d\n", i + k, num_size);
				return -1;
			}

			curr_flip ^= 1;
			nums[i] += 2;

			++num_flip;

//			printf("curr %d i %d num %d num_flip %d\n", curr_flip, i, nums[i], num_flip);
		}
	}

	return num_flip;
}

int main(int argc, char *argv[])
{
//	int nums[] = {0, 0, 0, 1, 0, 1, 1, 0};
//	int k = 3; // 3

//	int nums[] = {0,1,0};
//	int k = 1; // 2

//	int nums[] = {1,1,0};
//	int k = 2; // -1

//	int nums[] = {1,1};
//	int k = 2; // 0

	int nums[] = {0,1,0,0,1,0};
	int k = 4; // 3

	printf("%d\n", minKBitFlips(nums, sizeof(nums)/sizeof(nums[0]), k));

	return 0;
}

