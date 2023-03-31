
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include "../lib/helpers.h"

#include "../lib/hash.h"

struct Node
{
	struct Node *node1;
	struct Node *node2;
};

void NodeInit(struct Node *node)
{
	node->node1 = NULL;
	node->node2 = NULL;
}

struct Node* CopyNodeHelper(struct Node *node, struct Hash *hash)
{
	struct Node *new_node = NULL;

	if (node == NULL)
		return NULL;

	printf("hash search %p\n", node);

	if ((new_node = HashGet(hash, node)) != NULL)
		return new_node;

	new_node = calloc(1, sizeof(struct Node));

	printf("hash inert %p -> %p\n", node, new_node);
	HashInsert(hash, node, new_node);

	new_node->node1 = CopyNodeHelper(node->node1, hash);
	new_node->node2 = CopyNodeHelper(node->node2, hash);

	return new_node;
}

int FuncNodeToInt(void *obj)
{
	struct Node *node = obj;

//	printf("node %p intptr_t %ld int %d\n", node, (intptr_t)node, (int)((intptr_t)node));

	return (intptr_t)node;
}

int FuncNodeCompare(void *obj1, void *obj2)
{
	int val1 = FuncNodeToInt(obj1);
	int val2 = FuncNodeToInt(obj2);

//	printf("val1 %d val2 %d\n", val1, val2);

	return FuncIntCompare(&val1, &val2);
}

struct Node* CopyNode(struct Node *node)
{
	struct Hash hash;
	HashInit(&hash, FuncNodeToInt, FuncNodeCompare);

	return CopyNodeHelper(node, &hash);
}

int main(int argc, char *argv[])
{
	struct Node node;
	NodeInit(&node);

	node.node1 = calloc(1, sizeof(struct Node));
	NodeInit(node.node1);

	node.node2 = calloc(1, sizeof(struct Node));
	NodeInit(node.node2);

	node.node1->node1 = node.node1;
	node.node1->node2 = node.node2;

	printf("parent %p self %p child %p\n", &node, node.node1, node.node1->node1);

	CopyNode(&node);

	return 0;
}

