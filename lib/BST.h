
#ifndef BST_H_
#define BST_H_

#include "BST_node.h"

struct BST // Binary Search Tree
{
	struct BSTNode *root;
};

int BSTIsBalance(struct BST *bst);
int BSTHeight(struct BST *bst);

void BSTReplace(struct BST *bst, struct BSTNode *from, struct BSTNode *to);

struct BSTNode* BSTSearch(struct BST *bst, int val);

struct BSTNode* BSTDeleteNode(struct BST *bst, struct BSTNode *bst_node);
struct BSTNode* BSTDelete(struct BST *bst, int val);

struct BSTNode* BSTInsert(struct BST *bst, int val, void *user_data);

int BSTIsValid(struct BST *bst);
void BSTDump(struct BST *bst);

void BSTInit(struct BST *bst);

// ====================

struct BST* MakeMinimalBST(int *nums_sorted, int num_size);

#endif
