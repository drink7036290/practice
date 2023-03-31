
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "../lib/helpers.h"

struct Range
{
	int *nums;
	int num_size;

	int start;
	int end; // inclusive

	int is_neg;
};

int RangeIsValidIndex(struct Range *range, int index)
{
	return ((index >= 0) && (index < range->num_size));
}

long long RangeLength(struct Range *range)
{
	if (! RangeIsValidIndex(range, range->start))
		return 0;

	if (! RangeIsValidIndex(range, range->end))
		return 0;

	return (long long)(range->end - range->start + 1);
}

void RangeDump(struct Range *range)
{
	printf("start %d end %d\n", range->start, range->end);
}

void RangeInit(struct Range *range, int *nums, int num_size, int is_neg)
{
	range->nums = nums;
	range->num_size = num_size;

	range->start = 0;
	range->end = num_size - 1;

	range->is_neg = is_neg;
}

struct RangePair
{
	struct Range *r1;
	struct Range *r2;
};

void RangePairInit(struct RangePair *range_pair, struct Range *r1, struct Range *r2)
{
	range_pair->r1 = r1;
	range_pair->r2 = r2;
}

int LeftMostOrClosestIndex(int *nums, int num_size, int target)
{
	int left = 0, right = num_size - 1;
	int index = INT_MAX, index_close = -1;

	while (left <= right)
	{
		int mid = (left + right) / 2;
		int mid_val = nums[mid];

//		printf("mid %d val %d target %d index %d close %d\n", mid, mid_val, target, index, index_close);

		if (mid_val == target)
			index = Min(index, mid);

		if (mid_val >= target)
			right = mid - 1;
		else
		{
			index_close = mid;
			left = mid + 1;
		}
	}

	if (index == INT_MAX)
		index = index_close;

	return index;
}

void SplitNegative(int *nums, int num_size, struct Range *r_neg, struct Range *r_pos)
{
	int index0 = LeftMostOrClosestIndex(nums, num_size, 0);

//	printf("index0 %d\n", index0);

	RangeInit(r_neg, nums, num_size, 1);
	RangeInit(r_pos, nums, num_size, 0);

	if (index0 == -1) // no negative
	{ 
		r_neg->start = INT_MAX;
		r_neg->end = INT_MIN;
	}
	else if (index0 == num_size - 1) // no positive
	{
		r_pos->start = INT_MAX;
		r_pos->end = INT_MIN;
	}
	else
	{
		if (nums[index0] == 0)
		{
			r_neg->end = index0 - 1;
			r_pos->start = index0;
		}
		else
		{
			r_neg->end = index0;
			r_pos->start = index0 + 1;
		}
	}
}

long long GetNumberOfProducts(struct RangePair *pair, long long max_product, long long *actual_max_product)
{
	struct Range *r1 = pair->r1, *r2 = pair->r2;

	// 2 iterators
	int i = r1->start;
	int j = r2->end;

	long long count = 0;

	if (r1->is_neg == r2->is_neg)
	{
		printf("i %d j %d max_product %lld\n", i, j, max_product);

		while ((i <= r1->end) && (j >= r2->start))
		{
			long long product = ((long long)r1->nums[i]) * r2->nums[j];

			printf("i %d j %d r1 %d r2 %d product %lld\n", i, j, r1->nums[i], r2->nums[j], product);

			if (product > max_product)
			{
				if (r1->is_neg)
					++i;
				else
					--j;
			}
			else // <=
			{
				*actual_max_product = MaxLL(*actual_max_product, product);

				if (r1->is_neg)
				{
					count += r1->end - i + 1;
					--j;
				}
				else
				{ 
					count += j - r2->start + 1;
					++i;
				}
			}
		}
	}
	else
	{
		j = r2->start;

		printf("i %d j %d max_product %lld\n", i, j, max_product);

		while ((i <= r1->end) && (j <= r2->end))
		{
			long long product = ((long long)r1->nums[i]) * r2->nums[j];

			printf("i %d j %d r1 %d r2 %d product %lld\n", i, j, r1->nums[i], r2->nums[j], product);

			if (product > max_product)
			{
				if (r1->is_neg)
					++j;
				else
					++i;
			}
			else // <=
			{
				*actual_max_product = MaxLL(*actual_max_product, product);

				if (r1->is_neg)
				{
					count += r2->end - j + 1;
					++i;
				}
				else
				{ 
					count += r1->end - i + 1;
					++j;
				}
			}
		}
	}

	printf("count %lld\n", count);

	return count;
}

long long BinarySearch(struct RangePair *pair1, struct RangePair *pair2
		, long long min, long long max, long long k)
{
	long long left = min, right = max;

	long long last_max_product1 = LLONG_MIN, last_max_product2 = LLONG_MIN;

	while (left <= right)
	{
		long long mid = (left + right) / 2;

		long long num1 = 0, num2 = 0, num = 0;

		long long actual_max_product1 = LLONG_MIN, actual_max_product2 = LLONG_MIN;

		num1 = GetNumberOfProducts(pair1, mid, &actual_max_product1);
		num2 = GetNumberOfProducts(pair2, mid, &actual_max_product2);

		num = num1 + num2;

		printf("left %lld mid %lld right %lld num1 %lld num2 %lld k %lld\n", left, mid, right, num1, num2, k);

		if (num == k)
			return MaxLL(actual_max_product1, actual_max_product2);

		if (num > k)
		{
			last_max_product1 = actual_max_product1;
			last_max_product2 = actual_max_product2;

			right = mid - 1;
		}
		else
			left = mid + 1;
	}

	return MaxLL(last_max_product1, last_max_product2);
}

long long KthSmallestProduct(int *nums1, int num_size1, int *nums2, int num_size2, long long k)
{
	struct Range r1_neg, r1_pos, r2_neg, r2_pos;
	struct RangePair pair1, pair2;
	long long num_neg = 0;

	SplitNegative(nums1, num_size1, &r1_neg, &r1_pos);
	SplitNegative(nums2, num_size2, &r2_neg, &r2_pos);

	RangeDump(&r1_neg);
	RangeDump(&r1_pos);
	RangeDump(&r2_neg);
	RangeDump(&r2_pos);

	num_neg = RangeLength(&r1_neg) * RangeLength(&r2_pos)
		+ RangeLength(&r1_pos) * RangeLength(&r2_neg);

	printf("num_neg %lld\n", num_neg);

	if (num_neg >= k)
	{
		RangePairInit(&pair1, &r1_neg, &r2_pos);
		RangePairInit(&pair2, &r1_pos, &r2_neg);

		return BinarySearch(&pair1, &pair2, LLONG_MIN, 0, k); // NOT LLONG_MIN, -1
	}

	printf("k - num_neg %lld\n", k - num_neg);

	RangePairInit(&pair1, &r1_pos, &r2_pos);
	RangePairInit(&pair2, &r1_neg, &r2_neg);

	return BinarySearch(&pair1, &pair2, 0, LLONG_MAX, k - num_neg);
}

int main(int argc, char *argv[])
{
//#define kNumSize1 2
//	int nums1[kNumSize1] = {2, 5};
//#define kNumSize2 2
//	int nums2[kNumSize2] = {3, 4};
//	long long k = 2; // ans: 8

//#define kNumSize1 4
//	int nums1[kNumSize1] = {-4,-2,0,3};
//#define kNumSize2 2
//	int nums2[kNumSize2] = {2, 4};
//	long long k = 6; // ans: 0

//#define kNumSize1 5
//	int nums1[kNumSize1] = {-2,-1,0,1,2};
//#define kNumSize2 5
//	int nums2[kNumSize2] = {-3,-1,2,4,5};
//	long long k = 3; // ans: -6

//#define kNumSize1 3
//	int nums1[kNumSize1] = {1,2,3};
//#define kNumSize2 2
//	int nums2[kNumSize2] = {-1,1};
//	long long k = 4; // ans: 1

//#define kNumSize1 4
//        int nums1[kNumSize1] = {-4, -2, 0, 3};
//#define kNumSize2 2
//        int nums2[kNumSize2] = {1,10};
//        long long k = 7; // ans: 3

//#define kNumSize1 2
//        int nums1[kNumSize1] = {-100000,100000};
//#define kNumSize2 2
//        int nums2[kNumSize2] = {-100000,100000};
//        long long k = 1; // ans: -10000000000

#define kNumSize1 4
        int nums1[kNumSize1] = {-8,-8,3,7};
#define kNumSize2 1
        int nums2[kNumSize2] = {-1};
        long long k = 3; // ans: 8

//#define kNumSize1 4
//        int nums1[kNumSize1] = {-3,-1,5,6};
//#define kNumSize2 9
//        int nums2[kNumSize2] = {-10,-7,-6,-5,-5,-4,-1,7,8};
//        long long k = 28; // ans: 15

	printf("%lld\n", KthSmallestProduct(nums1, kNumSize1, nums2, kNumSize2, k));

	return 0;
}

