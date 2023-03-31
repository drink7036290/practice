
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "BST.h"

int BSTIsBalance(struct BST *bst)
{
	int unused = 0;

	return BSTNodeIsBalance(bst->root, &unused);
}

int BSTHeight(struct BST *bst)
{
	return BSTNodeHeight(bst->root);
}

void BSTReplace(struct BST *bst, struct BSTNode *from, struct BSTNode *to)
{
	struct BSTNode *from_parent = from->parent;

	if (from_parent == NULL)
		bst->root = to;
	else if (from == from_parent->left)
		from_parent->left = to;
	else
		from_parent->right = to;

	if (to != NULL)
		to->parent = from_parent;
}

struct BSTNode* BSTSearch(struct BST *bst, int val)
{
	struct BSTNode *bst_node = bst->root;

	while (bst_node != NULL)
	{
		if (val == bst_node->val)
			return bst_node;

		if (val < bst_node->val)
			bst_node = bst_node->left;
		else
			bst_node = bst_node->right;
	}

	return NULL;
}

struct BSTNode* BSTDeleteNode(struct BST *bst, struct BSTNode *bst_node)
{
	struct BSTNode *successor = NULL;

	if (bst_node == NULL)
		return NULL;

	if (bst_node->left == NULL)
	{
		BSTReplace(bst, bst_node, bst_node->right);
		return bst_node;
	}

	if (bst_node->right == NULL)
	{
		BSTReplace(bst, bst_node, bst_node->left);
		return bst_node;
	}

	successor = BSTNodeMin(bst_node->right);

	if (successor != bst_node->right)
	{
		BSTReplace(bst, successor, successor->right);
		successor->right = bst_node->right;
		bst_node->right->parent = successor;
	}

	BSTReplace(bst, bst_node, successor);
	successor->left = bst_node->left;
	bst_node->left->parent = successor;

	return bst_node;
}

struct BSTNode* BSTDelete(struct BST *bst, int val)
{
	struct BSTNode *bst_node = BSTDeleteNode(bst, BSTSearch(bst, val));

	BSTNodeReset(bst_node);

	return bst_node;
}

struct BSTNode* BSTInsert(struct BST *bst, int val, void *user_data)
{
	struct BSTNode *bst_node = bst->root, *parent = NULL;

	while (bst_node != NULL)
	{
		parent = bst_node;

		if (val <= bst_node->val)
			bst_node = bst_node->left;
		else
			bst_node = bst_node->right;
	}

	bst_node = calloc(1, sizeof(struct BSTNode));
	BSTNodeInit(bst_node, val, user_data);

	if (parent == NULL) // tree empty
		bst->root = bst_node;
	else
		BSTNodeConnect(bst_node, parent);

	// [TEST]
	//bst_node->val = rand() % 100;

	return bst_node;
}

int BSTIsValid(struct BST *bst)
{
	int min = INT_MIN, max = INT_MAX;

	return BSTNodeIsValid(bst->root, min, max);
}

void BSTDump(struct BST *bst)
{
	printf("height %d balance %d\n", BSTHeight(bst), BSTIsBalance(bst));
	BSTNodeDump(bst->root, 0);

	if (! BSTIsValid(bst))
		printf("Not a valid BST\n");
	else
		printf("Valid BST\n");
}

void BSTInit(struct BST *bst)
{
	bst->root = NULL;
}

// ====================

struct BSTNode* MakeMinimalBSTHelper(int *nums_sorted, int start, int end)
{
	int half = -1;
	struct BSTNode *self = NULL;

	if (start > end)
		return NULL;

	half = (start + end) / 2;

	self = calloc(1, sizeof(struct BSTNode));
	BSTNodeInit(self, nums_sorted[half], NULL);

	self->left = MakeMinimalBSTHelper(nums_sorted, start, half - 1);
	self->right = MakeMinimalBSTHelper(nums_sorted, half + 1, end);

	if (self->left != NULL)
		self->left->parent = self;
	if (self->right != NULL)
		self->right->parent = self;

	return self;
}

struct BST* MakeMinimalBST(int *nums_sorted, int num_size)
{
	struct BST *bst = calloc(1, sizeof(struct BST));

	BSTInit(bst);

	bst->root = MakeMinimalBSTHelper(nums_sorted, 0, num_size - 1);

	return bst;
}

