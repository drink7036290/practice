
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "../lib/pair.h"
#include "../lib/helpers.h"

struct Range
{
	int *nums;
	int num_size;

	int start;
	int end;
};

int RangeIsValidIndex(struct Range *range, int index)
{
	return ((index >= 0) && (index < range->num_size));
}

void RangeSetMidPair(struct Range *range, int middle, struct Pair *pair)
{
	if (RangeIsValidIndex(range, middle))
		pair->x = range->nums[middle];

	if (RangeIsValidIndex(range, middle + 1))
		pair->y = range->nums[middle + 1];
}

void RangeInit(struct Range *range, int *nums, int num_size)
{
	range->nums = nums;
	range->num_size = num_size;

	range->start = -1; // left most - 1
	range->end = num_size; // right most + 1
}

int BinarySearch(struct Range *range1, struct Range *range2, int k)
{
	while (range1->start <= range1->end)
	{
		int middle1 = (range1->start + range1->end) / 2, middle2 = INT_MIN;
		int count1 = middle1 + 1;

		struct Pair pair1, pair2;

		if (count1 > k)
		{
			range1->end = middle1 - 1;
			continue;
		}

		middle2 = k - count1 - 1;

		printf("start1 %d middle1 %d end1 %d/ middle2 %d\n", range1->start, middle1, range1->end, middle2);

		PairInit(&pair1, INT_MIN, INT_MAX);
		PairInit(&pair2, INT_MIN, INT_MAX);

		RangeSetMidPair(range1, middle1, &pair1);
		RangeSetMidPair(range2, middle2, &pair2);

		if (pair1.x > pair2.y)
		{
			range1->end = middle1 - 1;
			continue;
		}

		if (pair2.x > pair1.y)
		{
			range1->start = middle1 + 1;
			continue;
		}

		return Max(pair1.x, pair2.x);
	}

	return INT_MIN; // error
}

int FindKth(int *nums1, int num_size1, int *nums2, int num_size2, int k)
{
	struct Range range1, range2;

	if ((k <= 0) || (k > num_size1 + num_size2)) // not allowed
		return INT_MIN; // error

	if (num_size1 <= 0)
		return nums2[k - 1];
	if (num_size2 <= 0)
		return nums1[k - 1];

	RangeInit(&range1, nums1, Min(num_size1, k));
	RangeInit(&range2, nums2, Min(num_size2, k));

	if (num_size1 <= num_size2)
		return BinarySearch(&range1, &range2, k);

	return BinarySearch(&range2, &range1, k);
}

#define kNumSize1 5
#define kNumSize2 5

int main(int argc, char *argv[])
{
	int nums1[kNumSize1] = {1, 2, 3, 4, 5};
	int nums2[kNumSize2] = {6, 7, 8, 9, 10};
	int k = 5;

	printf("%d\n", FindKth(nums1,  kNumSize1, nums2, kNumSize2, k));

	return 0;
}

