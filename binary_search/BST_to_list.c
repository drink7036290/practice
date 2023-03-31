
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../lib/helpers.h"

#include "../lib/BST.h"

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

void BSTToListHelper(struct BSTNode *bst_node, struct ListNode *list)
{
	struct ValInfo *val_info = NULL;

	if (bst_node == NULL)
		return;

	BSTToListHelper(bst_node->left, list);

	val_info = calloc(1, sizeof(struct ValInfo));
	ValInfoInit(val_info, bst_node->val);

	ListAddTail(list, &val_info->hook);

	BSTToListHelper(bst_node->right, list);
}

struct ListNode* BSTToList(struct BST *bst)
{
	struct ListNode *list = calloc(1, sizeof(struct ListNode));
	ListInit(list);

	BSTToListHelper(bst->root, list);

	return list;
}

void DumpList(struct ListNode* list)
{
	struct ListNode *head = list, *p = NULL;

	for (p = head->next; p != head; p = p->next)
	{
		struct ValInfo *val_info = p->container;

		ValInfoDump(val_info);
	}
	printf("\n");
}

void Test(int num)
{
	struct BST bst;

	int last_val = -1;

	BSTInit(&bst);

	srand(time(NULL));

	while (num > 0)
	{
		int val = rand() % 100;

		BSTInsert(&bst, val, NULL);

		if (last_val == -1)
			last_val = val;

		--num;
	}

	BSTDump(&bst);

	DumpList(BSTToList(&bst));
}

int main(int argc, char *argv[])
{
	Test(atoi(argv[1]));

	return 0;
}

