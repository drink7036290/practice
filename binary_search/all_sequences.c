
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../lib/helpers.h"

#include "../lib/BST.h"

struct IntInfo
{
	int val;
	struct ListNode hook;
};

void IntInfoInit(struct IntInfo *int_info, int val)
{
	int_info->val = val;
	ListNodeInit(&int_info->hook, int_info);
}

struct ListInfo
{
	struct ListNode list;
	struct ListNode hook;
};

void ListInfoInit(struct ListInfo *list_info)
{
	ListInit(&list_info->list);
	ListNodeInit(&list_info->hook, list_info);
}

void DumpSequence(struct ListNode *list)
{
	struct ListNode *head = list, *p = NULL;

	for (p = head->next; p != head; p = p->next)
	{
		struct IntInfo *int_info = p->container;
		printf(" %d", int_info->val);
	}
	printf("\n");
}

void CloneList(struct ListNode *list, struct ListNode *list_new)
{
	struct ListNode *head = list, *p = NULL;

	for (p = head->next; p != head; p = p->next)
	{
		struct IntInfo *int_info = p->container;

		struct IntInfo *int_info_new = calloc(1, sizeof(struct IntInfo));
		IntInfoInit(int_info_new, int_info->val);

		ListAddTail(list_new, &int_info_new->hook);
	}
}

void BuildList(struct ListNode *list1, struct ListNode *list2, struct ListNode *list_prefix, struct ListNode *list_result)
{
	struct ListInfo *list_info = calloc(1, sizeof(struct ListInfo));
	ListInfoInit(list_info);

	CloneList(list_prefix, &list_info->list);
	CloneList(list1, &list_info->list);
	CloneList(list2, &list_info->list);

	ListAddTail(list_result, &list_info->hook);
}

void WeaveList(struct ListNode *list1, struct ListNode *list2, struct ListNode *list_prefix, struct ListNode *list_result)
{
	struct ListNode *list_node = NULL;

	if (ListIsEmpty(list1) || ListIsEmpty(list2))
	{
		BuildList(list1, list2, list_prefix, list_result);
		return;
	}

	list_node = ListPopFront(list1);
	ListAddTail(list_prefix, list_node);

	WeaveList(list1, list2, list_prefix, list_result);

	ListNodeDelete(list_node);
	ListAddFront(list1, list_node);

	// =====

	list_node = ListPopFront(list2);
	ListAddTail(list_prefix, list_node);

	WeaveList(list1, list2, list_prefix, list_result);

	ListNodeDelete(list_node);
	ListAddFront(list2, list_node);
}

void DumpSequences(struct ListNode *list)
{
	struct ListNode *head = list, *p = NULL;

	for (p = head->next; p != head; p = p->next)
	{
		struct ListInfo *list_info = p->container;
		DumpSequence(&list_info->list);
	}
}

void WeaveLists(struct ListNode *list1, struct ListNode *list2, struct ListNode *list_prefix, struct ListNode *list_result)
{
	struct ListNode *head1 = list1, *p1 = NULL;
	struct ListNode *head2 = list2, *p2 = NULL;

	for (p1 = head1->next; p1 != head1; p1 = p1->next)
	{
		struct ListInfo *list_info1 = p1->container;

		for (p2 = head2->next; p2 != head2; p2 = p2->next)
		{
			struct ListInfo *list_info2 = p2->container;

			WeaveList(&list_info1->list, &list_info2->list, list_prefix, list_result);
		}
	}
}

struct ListNode* AllSequences(struct BSTNode *bst_node)
{
	struct IntInfo *int_info = NULL;
	struct ListNode list_prefix;

	struct ListNode *list_left = NULL, *list_right = NULL;

	struct ListNode *list_result = calloc(1, sizeof(struct ListNode));
	ListInit(list_result);

	if (bst_node == NULL)
	{
		struct ListInfo *list_info = calloc(1, sizeof(struct ListInfo));
		ListInfoInit(list_info);

		ListAddTail(list_result, &list_info->hook); // [IMPORTANT !!] Add an empty list

		return list_result;
	}

	int_info = calloc(1, sizeof(struct IntInfo));
	IntInfoInit(int_info, bst_node->val);

	ListInit(&list_prefix);
	ListAddTail(&list_prefix, &int_info->hook);

	list_left = AllSequences(bst_node->left);
	list_right = AllSequences(bst_node->right);

	WeaveLists(list_left, list_right, &list_prefix, list_result);

	return list_result;
}

void TestBST(int num)
{
	struct BST bst;

	BSTInit(&bst);

	srand(time(NULL));

	while (num > 0)
	{
		int val = rand() % 100;

		BSTInsert(&bst, val, NULL);

		--num;
	}

	BSTDump(&bst);

	DumpSequences(AllSequences(bst.root));
}

int main(int argc, char *argv[])
{
	TestBST(atoi(argv[1]));

	return 0;
}

