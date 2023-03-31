
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../lib/helpers.h"

#include "../lib/hash.h"


struct Position
{
	int val;
	int index;
};

void PositionDump(struct Position *position)
{
	printf(" %d(%d)", position->val, position->index);
}

void PositionInit(struct Position *position, int val, int index)
{
	position->val = val;
	position->index = index;
}

struct Combination
{
	struct Position pos1;
	struct Position pos2;

	struct ListNode hook;
};

void CombinationDump(struct Combination *comb)
{
	PositionDump(&comb->pos1);
	PositionDump(&comb->pos2);
	printf("\n");
}

void CombinationInit(struct Combination *comb, int val1, int index1, int val2, int index2)
{
	PositionInit(&comb->pos1, val1, index1);
	PositionInit(&comb->pos2, val2, index2);

	ListNodeInit(&comb->hook, comb);
}

struct ListNode* GetCombinations(int *nums, int num_size, int target)
{
	int i = 0;
	struct ListNode *list_comb = calloc(1, sizeof(struct ListNode));
	struct Hash hash;

	ListInit(list_comb);
	HashInit(&hash, FuncIntToInt, FuncIntCompare);

	for (i = 0; i < num_size; ++i)
	{
		int val = target - nums[i];
		struct Position *pos = NULL;

		if ((pos = HashGet(&hash, &val)) == NULL)
		{
			pos = calloc(1, sizeof(struct Position));
			PositionInit(pos, nums[i], i);

			HashInsert(&hash, &nums[i], pos);
		}
		else
		{
			struct Combination *comb = calloc(1, sizeof(struct Combination));
			CombinationInit(comb, pos->val, pos->index, nums[i], i);

			ListAddTail(list_comb, &comb->hook);
		}
	}

	return list_comb;
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

void Test(int num)
{
	int *nums = calloc(num, sizeof(int));
	int i = 0, target = 0;
	
	struct ListNode *list_comb = NULL;

	srand(time(NULL));

	target = rand() % 100;
	target = 6;
	printf("target %d\n", target);

	for (i = 0; i < num; ++i)
		nums[i] = rand() % 100;

	nums[0] = 3;
	nums[1] = 2;
	nums[2] = 4;
	nums[3] = 15;

	for (i = 0; i < num; ++i)
		printf(" %d", nums[i]);
	printf("\n");

	list_comb = GetCombinations(nums, num, target);

	DumpList(list_comb);
}

int main(int argc, char *argv[])
{
	Test(atoi(argv[1]));

	return 0;
}

