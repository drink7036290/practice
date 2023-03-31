
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../lib/helpers.h"

#include "../lib/list_node.h"

struct Info
{
	int number;

	struct ListNode hook;	
};

void InfoDump(struct Info *info)
{
	printf(" %d", info->number);
}

void InfoInit(struct Info *info, int number)
{
	info->number = number;

	ListNodeInit(&info->hook, info);
}

void ListDump(struct ListNode *list)
{
	struct ListNode *head = list, *p = NULL;

	printf("===\n");
	for (p = head->next; p != head; p = p->next)
	{
		struct Info *info = p->container;
		InfoDump(info);
	}
	printf("\n===\n");
}

void ListRearrange(struct ListNode *list)
{
	struct ListNode *head = list, *mid = NULL;
	struct ListNode *start = head->next, *p = NULL, *n = NULL;

	mid = ListGetRangeMiddle(head->next, head->prev);

	for (p = mid->next, n = p->next; p != head; p = n, n = p->next)
	{
		ListNodeDelete(p);
		ListAddFront(start, p);

		start = start->next;
		start = start->next;
	}
}

void BuildList(struct ListNode *list, int *nums, int num_size)
{
	int i = 0;

	for (i = 0; i < num_size; ++i)
	{
		struct Info *info = calloc(1, sizeof(struct Info));

		InfoInit(info, nums[i]);

		ListAddTail(list, &info->hook);
	}
}

void ListInterWeaving(int *nums, int num_size)
{
	struct ListNode list;

	ListInit(&list);

	BuildList(&list, nums, num_size);

	ListDump(&list);

	ListRearrange(&list);

	ListDump(&list);
}

int main(int argc, char *argv[])
{
#define kNumSize 7	
	int nums[kNumSize] = {1, 2, 3, 4, 5, 6, 7};

	ListInterWeaving(nums, kNumSize);

	return 0;
}

