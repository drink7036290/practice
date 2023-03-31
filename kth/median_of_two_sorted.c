
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "../lib/pair.h"
#include "../lib/helpers.h"

struct Range
{
	int *nums;
	int nums_size;

	int start;
	int end;
};

double RangeGetMedian(struct Range *range)
{
	int half = (range->start + range->end) / 2;

	if (range->nums_size == 0)
		return 0;

	if (range->nums_size % 2 == 0)
		return (range->nums[half] + range->nums[half + 1]) / 2.; // int to float

	return range->nums[half];
}

int RangeIsValidIndex(struct Range *range, int index)
{
	return ((index >= 0) && (index < range->nums_size));
}

void RangeSetMidPair(struct Range *range, int mid, struct Pair *pair)
{
	if (RangeIsValidIndex(range, mid))
		pair->x = range->nums[mid];

	if (RangeIsValidIndex(range, mid + 1))
		pair->y = range->nums[mid + 1];
}

void RangeInit(struct Range *range, int *nums, int nums_size)
{
	range->nums = nums;
	range->nums_size = nums_size;

	range->start = -1; // [TRICKEY] left - 1
	range->end = nums_size; // [TRICKEY] right + 1
}

double MedianOfTwoRanges(struct Range *range1, struct Range *range2)
{
	int half = (range1->nums_size + range2->nums_size) / 2;

	struct Pair mid_pair1, mid_pair2;

	while (range1->start <= range1->end)
	{
		int mid1 = (range1->start + range1->end) / 2;
		int count = mid1 + 1; // start from 0

		int mid2 = (half - count) - 1; // start from 0

		// reinit
		PairInit(&mid_pair1, INT_MIN, INT_MAX);
		PairInit(&mid_pair2, INT_MIN, INT_MAX);

		RangeSetMidPair(range1, mid1, &mid_pair1);
		RangeSetMidPair(range2, mid2, &mid_pair2);
/*
		printf("index start1 %d mid1 %d end1 %d mid2 %d / mid_pair1 %d %d mid_pair2 %d %d\n"
			, range1->start, mid1, range1->end, mid2, mid_pair1.x, mid_pair1.y, mid_pair2.x, mid_pair2.y);
*/
		if (mid_pair2.x > mid_pair1.y)
		{
			range1->start = mid1 + 1;
			continue;
		}

		if (mid_pair1.x > mid_pair2.y)
		{
			range1->end = mid1 - 1;
			continue;
		}

		if ((range1->nums_size + range2->nums_size) % 2 == 0)
		{
			int left_max = Max(mid_pair1.x, mid_pair2.x);
			int right_min = Min(mid_pair1.y, mid_pair2.y);

			return (left_max + right_min) / 2.; // int to float
		}

		return Min(mid_pair1.y, mid_pair2.y);
	}
	
	return 0; // error
}

double MedianOfTwoSorted(int *nums1, int nums1_size, int *nums2, int nums2_size)
{
	struct Range range1, range2;
	
	RangeInit(&range1, nums1, nums1_size);
	RangeInit(&range2, nums2, nums2_size);

	if (nums1_size == 0)
                return RangeGetMedian(&range2);

        if (nums2_size == 0)
                return RangeGetMedian(&range1);

	if (nums1_size <= nums2_size)
		return MedianOfTwoRanges(&range1, &range2);

	return MedianOfTwoRanges(&range2, &range1);
}

#define kNumsSize1 2
#define kNumsSize2 1

int main(int argc, char *argv[])
{
	int nums1[kNumsSize1] = {1, 3};
	int nums2[kNumsSize2] = {2};

//	int nums1[kNumsSize1] = {3};
//	int nums2[kNumsSize2] = {-2, -1};

//	int nums1[kNumsSize1] = {2};
//	int nums2[kNumsSize2] = {1, 3, 4, 5, 6};

	printf("%lf\n", MedianOfTwoSorted(nums1, kNumsSize1, nums2, kNumsSize2));

	return 0;
}

