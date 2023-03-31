
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../lib/helpers.h"

#include "../lib/list_node.h"

struct Combination
{
	int *vals;
	int size;

	struct ListNode hook;
};

void CombinationDump(struct Combination *comb)
{
	int i = 0;

	for (i = 0; i < comb->size; ++i)
		printf(" %d", comb->vals[i]);
	printf("\n");
}

void CombinationInit(struct Combination *comb, int k)
{
	comb->size = k;
	comb->vals = calloc(k, sizeof(int));

	ListNodeInit(&comb->hook, comb);
}

struct Params
{
	int *nums;
	int num_size;

	int k;

	int *vals;
	int index_pos;

	struct ListNode *list_comb;
};

void ParamsInit(struct Params *params, int *nums, int num_size, int k)
{
	params->nums = nums;
	params->num_size = num_size;

	params->k = k;

	params->vals = calloc(k, sizeof(int));
	params->index_pos = -1; // -1

	params->list_comb = calloc(1, sizeof(struct ListNode));
	ListInit(params->list_comb);
}

void TwoSum(struct Params *params, int target, int index)
{
	int i = index, j = params->num_size - 1;

	printf("%s target %d index %d\n", __FUNCTION__, target, index);

	while (i < j)
	{
		int sum = 0;
/*
		if (i > index)
		{
			if (params->nums[i - 1] == params->nums[i])
			{
				++i;
				continue;
			}
		}
		
		if (j < params->num_size - 1)
		{
			if (params->nums[j] == params->nums[j + 1])
			{
				--j;
				continue;
			}
		}
*/
		sum = params->nums[i] + params->nums[j];

		printf("sum %d target %d / %d %d\n", sum, target, params->nums[i], params->nums[j]);

		if (sum > target)
		{
			--j;
			continue;
		}
		
		if (sum == target)
		{
			int m = 0;

			struct Combination *comb = calloc(1, sizeof(struct Combination));
			CombinationInit(comb, params->k);

			for (m = 0; m < params->k - 2; ++m)
				comb->vals[m] = params->vals[m];

			comb->vals[m] = params->nums[i];
			comb->vals[m + 1] = params->nums[j];

			ListAddTail(params->list_comb, &comb->hook);
		}

		++i;
	}
}

void KSumHelper(struct Params *params, int target, int index, int k)
{
	int i = 0;

	if (k == 2)
	{
		TwoSum(params, target, index);
		return;
	}

	for (i = index; i < params->num_size; ++i)
	{
/*		if (i > index)
		{
			if (params->nums[i - 1] == params->nums[i])
				continue;
		}
*/
		params->vals[++params->index_pos] = params->nums[i];

		KSumHelper(params, target - params->nums[i], i + 1, k - 1);

		--params->index_pos;
	}
}

struct ListNode* KSum(int *nums, int num_size, int target, int k)
{
	struct Params params;

	if (k > num_size)
		return NULL;

	ParamsInit(&params, nums, num_size, k);

	ArrayQuickSort(nums, num_size, SortTypeNonDecreasing, FuncIntCompare, FuncIntDump);

	{
		int i = 0;
		for (i = 0; i < num_size; ++i)
			printf(" %d", nums[i]);
		printf("\n\n");
	}

	KSumHelper(&params, target, 0, k);

	return params.list_comb;
}

void DumpList(struct ListNode *list)
{
	struct ListNode *head = list, *p = NULL;

	for (p = head->next; p != head; p = p->next)
	{
		struct Combination *comb = p->container;

		CombinationDump(comb);
	}
	printf("\n");
}

void Test(void)
{
//	int nums[] = {2, 7, 11, 15};
//	int target = 9;
//	int k = 2;

//	int nums[] = {3, 2, 4};
//	int target = 6;
//	int k = 2;

//	int nums[] = {-1, 0, 1, 2, -1, -4};
//	int target = 0;
//	int k = 3;

	int nums[] = {1, 0, -1, 0, -2, 2};
	int target = 0;
	int k = 4;

	struct ListNode *list_comb = NULL;

	srand(time(NULL));

	list_comb = KSum(nums, sizeof(nums)/sizeof(nums[0]), target, k);

	DumpList(list_comb);
}

int main(int argc, char *argv[])
{
	Test();

	return 0;
}

