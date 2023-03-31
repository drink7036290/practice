
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "lib/helpers.h"

#include "lib/list_node.h"

struct ValInfo
{
	int val;

	struct ListNode hook;
};

void ValInfoDump(struct ValInfo *val_info)
{
	printf(" %d", val_info->val);
}

void ValInfoInit(struct ValInfo *val_info, int val)
{
	val_info->val = val;

	ListNodeInit(&val_info->hook, val_info);
}

struct PairInfo
{
	int val;
	int cn;

	struct ListNode hook;
};

void PairInfoDump(struct PairInfo *pair)
{
	printf(" %d(%d)", pair->val, pair->cn);
}

void PairInfoInit(struct PairInfo *pair, int val, int cn)
{
	pair->val = val;
	pair->cn = cn;

	ListNodeInit(&pair->hook, pair);
}

struct MinStack
{
	struct ListNode list;
	struct ListNode list_min; // save pair
};

int MinStackGetMin(struct MinStack *min_stack)
{
	struct ListNode *list_node = NULL;
	struct PairInfo *pair = NULL;

	if (ListIsEmpty(&min_stack->list_min))
		return INT_MIN; // error

	list_node = min_stack->list_min.next;
	pair = list_node->container;

	return pair->val;
}

int MinStackPop(struct MinStack *min_stack)
{
	struct ListNode *list_node = ListPopFront(&min_stack->list);
	struct ValInfo *val_info = NULL;
	int val = 0;

	struct PairInfo *pair = NULL;

	if (NULL == list_node)
		return INT_MIN; // error

	val_info = list_node->container;
	val = val_info->val;
	free(val_info);

	list_node = min_stack->list_min.next;
	pair = list_node->container;

	if (--pair->cn == 0)
	{
		ListPopFront(&min_stack->list_min);
		free(pair);
	}

	return val;
}

void MinStackPush(struct MinStack *min_stack, int val)
{
	struct PairInfo *pair = NULL;

	struct ValInfo *val_info = calloc(1, sizeof(struct ValInfo));
	ValInfoInit(val_info, val);

	ListAddFront(&min_stack->list, &val_info->hook);

	if (! ListIsEmpty(&min_stack->list_min))
	{
		struct ListNode *list_node = min_stack->list_min.next;
		pair = list_node->container;

		if (pair->val <= val)
		{
			++pair->cn;
			return;
		}
	}

	pair = calloc(1, sizeof(struct PairInfo));
	PairInfoInit(pair, val, 1);

	ListAddFront(&min_stack->list_min, &pair->hook);
}

void MinStackDump(struct MinStack *min_stack)
{
	struct ListNode *head = &min_stack->list, *p = NULL;

	printf("list ");
	for (p = head->next; p != head; p = p->next)
	{
		struct ValInfo *val_info = p->container;

		ValInfoDump(val_info);
	}
	printf("\n");

	head = &min_stack->list_min;

	printf("min %d/", MinStackGetMin(min_stack));
	for (p = head->next; p != head; p = p->next)
	{
		struct PairInfo *pair = p->container;

		PairInfoDump(pair);
	}
	printf("\n");
}

void MinStackInit(struct MinStack *min_stack)
{
	ListInit(&min_stack->list);
	ListInit(&min_stack->list_min);
}

void TestMinStack(int num)
{
	struct MinStack min_stack;
	int val = 0;

	MinStackInit(&min_stack);

	srand(time(NULL));

	while (num > 0)
	{
		MinStackPush(&min_stack, rand() % 100);

		MinStackDump(&min_stack);

		--num;
	}

	while ((val = MinStackPop(&min_stack)) != INT_MIN)
		MinStackDump(&min_stack);
}

int main(int argc, char *argv[])
{
	TestMinStack(atoi(argv[1]));

	return 0;
}

