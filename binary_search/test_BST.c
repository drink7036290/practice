
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include "../lib/helpers.h"
#include "../lib/BST.h"
#include "../lib/heap.h"

struct DepthInfo
{
	struct ListNode list;
	struct ListNode hook;
};

void DepthInfoInit(struct DepthInfo *depth_info)
{
	ListInit(&depth_info->list);
	ListNodeInit(&depth_info->hook, depth_info);
}

void DepthInfoDump(struct DepthInfo *depth_info)
{
	struct ListNode *head = &depth_info->list, *p = NULL;
        
        for (p = head->next; p != head; p = p->next)
	{
		struct BSTNode *bst_node = p->container;

		printf(" %d", bst_node->val);
	}
	printf("\n");
}

void DumpListDepth(struct ListNode* list_depth)
{
	struct ListNode *head = list_depth, *p = NULL;

	for (p = head->next; p != head; p = p->next)
	{
		struct DepthInfo *depth_info = p->container;

		DepthInfoDump(depth_info);
	}
}

void CollectChildren(struct ListNode *list_parent, struct ListNode *list_curr)
{
	struct ListNode *head = list_parent, *p = NULL;

	for (p = head->next; p != head; p = p->next)
	{
		struct BSTNode *bst_node = p->container;

		if (bst_node->left != NULL)
			ListAddTail(list_curr, &bst_node->left->hook);

		if (bst_node->right != NULL)
			ListAddTail(list_curr, &bst_node->right->hook);
	}
}

struct ListNode* GetListDepth(struct BSTNode *bst_node) // the other solution Hash + DFS
{
	struct ListNode *list_depth = calloc(1, sizeof(struct ListNode));
	struct DepthInfo *depth_info = NULL;

	ListInit(list_depth);

	if (bst_node == NULL)
		return list_depth;

	depth_info = calloc(1, sizeof(struct DepthInfo));
	DepthInfoInit(depth_info);

	ListAddTail(&depth_info->list, &bst_node->hook);

	while (! ListIsEmpty(&depth_info->list))
	{
		struct DepthInfo *depth_parent = depth_info;
		ListAddTail(list_depth, &depth_parent->hook);

		depth_info = calloc(1, sizeof(struct DepthInfo));
		DepthInfoInit(depth_info);

		CollectChildren(&depth_parent->list, &depth_info->list);
	}

	DumpListDepth(list_depth);

	return list_depth;
}

void TestBSTFromArray(int num)
{
	int *nums = calloc(num, sizeof(int));
	int i = 0;

	struct BST *bst = NULL;

	srand(time(NULL));

	for (i = 0; i < num; ++i)
		nums[i] = rand() % 100;

	HeapArraySort(nums, num, kMaxHeap);

	printf("%s\n", __FUNCTION__);
	for (i = 0; i < num; ++i)
		printf(" %d", nums[i]);
	printf("\n");

	bst = MakeMinimalBST(nums, num);

	BSTDump(bst);

	GetListDepth(bst->root);
}

void TestBST(int num)
{
	struct BST bst;
	struct BSTNode *bst_node = NULL;

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

	GetListDepth(bst.root);

	bst_node = BSTSearch(&bst, last_val);
	printf("last val %d search %d\n", last_val, bst_node->val);

	bst_node = BSTDelete(&bst, last_val);
	printf("last val %d del %d\n", last_val, bst_node->val);

//	BSTDump(&bst);

}

int main(int argc, char *argv[])
{
	TestBST(atoi(argv[1]));

	TestBSTFromArray(atoi(argv[1]));

	return 0;
}

