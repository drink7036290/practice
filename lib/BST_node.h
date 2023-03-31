
#ifndef BST_NODE_H_
#define BST_NODE_H_

#include "list_node.h"

struct BSTNode // Binary Search Tree Node
{
	int val;
	void *user_data;

	struct BSTNode *parent;

	struct BSTNode *left;
	struct BSTNode *right;

	int visited;
	struct ListNode hook;
};

int BSTNodeIsBalance(struct BSTNode *bst_node, int *ret_height);
int BSTNodeHeight(struct BSTNode *bst_node);

struct BSTNode* BSTNodeMin(struct BSTNode *bst_node);
struct BSTNode* BSTNodeMax(struct BSTNode *bst_node);

struct BSTNode* BSTNodeSuccessor(struct BSTNode *bst_node);
struct BSTNode* BSTNodePredecessor(struct BSTNode *bst_node);

void BSTNodeDisconnect(struct BSTNode *child);
void BSTNodeConnect(struct BSTNode *child, struct BSTNode *parent);

int BSTNodeIsValid(struct BSTNode *bst_node, int min, int max);
void BSTNodeDump(struct BSTNode *bst_node, int shift);

void BSTNodeReset(struct BSTNode *bst_node);
void BSTNodeInit(struct BSTNode *bst_node, int val, void *user_data);

#endif
