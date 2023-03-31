
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "helpers.h"
#include "BST_node.h"



int BSTNodeIsBalance(struct BSTNode *bst_node, int *ret_height)
{
	int left_height = 0, right_height = 0;

	if (bst_node == NULL)
	{
		*ret_height = 0;
		return 1;
	}

	if (! BSTNodeIsBalance(bst_node->left, &left_height))
		return 0;

	if (! BSTNodeIsBalance(bst_node->right, &right_height))
		return 0;

	if (abs(left_height - right_height) > 1)
		return 0;

	*ret_height = Max(left_height, right_height) + 1;

	return 1;
}

int BSTNodeHeight(struct BSTNode *bst_node)
{
	int left_height = 0, right_height = 0;

	if (bst_node == NULL)
		return 0;

	left_height = BSTNodeHeight(bst_node->left);
	right_height = BSTNodeHeight(bst_node->right);

	return Max(left_height, right_height) + 1;
}

struct BSTNode* BSTNodeMin(struct BSTNode *bst_node)
{
	while (bst_node->left != NULL)
		bst_node = bst_node->left;

	return bst_node;
}

struct BSTNode* BSTNodeMax(struct BSTNode *bst_node)
{
	while (bst_node->right != NULL)
		bst_node = bst_node->right;

	return bst_node;
}

struct BSTNode* BSTNodeSuccessor(struct BSTNode *bst_node)
{
	struct BSTNode *parent = NULL;

	if (bst_node->right != NULL)
		return BSTNodeMin(bst_node->right);

	while ((parent = bst_node->parent) != NULL)
	{
		if (parent->left == bst_node)
			return parent;

		bst_node = parent;
	}

	return NULL;
}

struct BSTNode* BSTNodePredecessor(struct BSTNode *bst_node)
{
	struct BSTNode *parent = NULL;

	if (bst_node->left != NULL)
		return BSTNodeMax(bst_node->left);

	while ((parent = bst_node->parent) != NULL)
	{
		if (parent->right == bst_node)
			return parent;

		bst_node = parent;
	}

	return NULL;
}

void BSTNodeDisconnect(struct BSTNode *child)
{
	if (child == NULL)
		return;

	if (child->parent == NULL)
		return;

	if (child->parent->left == child)
		child->parent->left = NULL;
	else
		child->parent->right = NULL;

	child->parent = NULL;
}

void BSTNodeConnect(struct BSTNode *child, struct BSTNode *parent)
{
	if (child->val <= parent->val)
		parent->left = child;
	else
		parent->right = child;

	child->parent = parent;
}

int BSTNodeIsValid(struct BSTNode *bst_node, int min, int max)
{
	if (bst_node == NULL)
		return 1;

	if ((bst_node->val <= min) || (bst_node->val > max))
		return 0;

	if (! BSTNodeIsValid(bst_node->left, min, bst_node->val))
		return 0;

	if (! BSTNodeIsValid(bst_node->right, bst_node->val, max))
		return 0;

	return 1;
}

void BSTNodeDump(struct BSTNode *bst_node, int shift)
{
	int i = 0;
	struct BSTNode *predecessor = NULL, *successor = NULL;

	if (bst_node == NULL)
		return;

	predecessor = BSTNodePredecessor(bst_node);
	successor = BSTNodeSuccessor(bst_node);
	
	for (i = 0; i < shift; ++i)
		printf("  ");

	printf("%d predecessor %d successor %d\n", bst_node->val
		, (predecessor == NULL) ? -1 : predecessor->val
		, (successor == NULL) ? -1 : successor->val);

	BSTNodeDump(bst_node->left, shift + 1);
	BSTNodeDump(bst_node->right, shift + 1);
}

void BSTNodeReset(struct BSTNode *bst_node)
{
	bst_node->parent = NULL;
	bst_node->left = NULL;
	bst_node->right = NULL;

	bst_node->visited = 0;
	ListNodeInit(&bst_node->hook, bst_node);
}

void BSTNodeInit(struct BSTNode *bst_node, int val, void *user_data)
{
	bst_node->val = val;
	bst_node->user_data = user_data;

	BSTNodeReset(bst_node);
}

