
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "../lib/helpers.h"

int GetUnitVolume(int *heights, int index, int *max)
{
	if (heights[index] < Max(*max, heights[index]))
		return *max - heights[index]; // width 1

	*max = heights[index];
	return 0;
}

int GetVolume(int *heights, int num_height)
{
	int left = 0, right = num_height - 1;
	int left_max = INT_MIN, right_max = INT_MIN;
	int volume = 0;

	while (left < right)
	{
		if (heights[left] == Min(heights[left], heights[right]))
		{
			volume += GetUnitVolume(heights, left, &left_max);

			++left;
		}
		else
		{
			volume += GetUnitVolume(heights, right, &right_max);

			--right;
		}
	}

	return volume;
}

int main(int argc, char *argv[])
{
//#define kNumHeight 15
//	int heights[kNumHeight] = {1,3,2,4,1,3,1,4,5,2,2,1,4,2,2};

//#define kNumHeight 16
//	int heights[kNumHeight] = {0, 0, 4, 0, 0, 6, 0, 0, 3, 0, 5, 0, 1, 0, 0, 0};

//#define kNumHeight 12
//	int heights[kNumHeight] = {0,1,0,2,1,0,1,3,2,1,2,1};

#define kNumHeight 6
	int heights[kNumHeight] = {4,2,0,3,2,5};

	printf("%d\n", GetVolume(heights, kNumHeight));

	return 0;
}

