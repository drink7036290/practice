
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "../lib/helpers.h"

int GetMostWater(int *heights, int num_height)
{
	int left = 0, right = num_height - 1;
	int max_volume = INT_MIN;

	while (left < right)
	{
		int min_height = Min(heights[left], heights[right]);

		max_volume = Max(max_volume, min_height * (right - left));

		if (heights[left] == min_height)
			++left;
		else
			--right;
	}

	return max_volume;
}

int main(int argc, char *argv[])
{
#define kNumHeight 9
	int heights[kNumHeight] = {1,8,6,2,5,4,8,3,7};

//#define kNumHeight 2
//        int heights[kNumHeight] = {1, 1};

	printf("%d\n", GetMostWater(heights, kNumHeight));

	return 0;
}

