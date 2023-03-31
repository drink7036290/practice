
#include <stdio.h>
#include <stdlib.h>

#include "trie_node.h"

#include "helpers.h"

int TrieNodeDepth(struct TrieNode *node)
{
	struct TrieNode *tmp = node;
	int depth = 0;

	while (tmp != NULL)
	{
		if (tmp->c == '\0') // root
			break;

		++depth;
		tmp = tmp->parent;
	}

	return depth;
}

char* TrieNodePathStr(struct TrieNode *node)
{
	int str_len = TrieNodeDepth(node);
	char *str = NULL;
	struct TrieNode *tmp = node;

	if (str_len == 0)
		return NULL;

	str = calloc(str_len + 1, sizeof(char)); // '\0'

	while (tmp != NULL)
	{
		if (tmp->c == '\0') // root
			break;

		str[str_len - 1] = tmp->c;
		--str_len;

		tmp = tmp->parent;
	}

	return str;
}

struct TrieNode* TrieNodeGet(struct TrieNode *node, char c)
{
	int key = c;

	if (node == NULL)
		return NULL;

	return HashGet(&node->children, &key);
}

struct TrieNode* TrieNodeAdd(struct TrieNode *node, char c)
{
	struct TrieNode *child = TrieNodeGet(node, c);

	if (child == NULL)
	{
		int *key = malloc(1 * sizeof(int));
		*key = c;

		child = malloc(1 * sizeof(struct TrieNode));
		TrieNodeInit(child, c, node);

		HashInsert(&node->children, key, child);
	}

	return child;
}

int TrieNodeIsEnd(struct TrieNode *node)
{
	return (TrieNodeGet(node, '\0') != NULL);
}

void TrieNodeMarkEnd(struct TrieNode *node)
{
	TrieNodeAdd(node, '\0');
}

void TrieNodeDump(struct TrieNode *node) // depth first
{
	struct ListNode *head = NULL, *p = NULL;

	if (node == NULL)
		return;

	printf("node %c\n", node->c);

	if (TrieNodeIsEnd(node))
		printf("%s\n", TrieNodePathStr(node));

	head = &node->children.list_nodes;

	for (p = head->next; p != head; p = p->next)
	{
		struct HashNode *hash_node = p->container;

		TrieNodeDump(hash_node->val);
	}
}

void TrieNodeExit(struct TrieNode *node)
{
	struct ListNode *head = NULL, *p = NULL;

	if (node == NULL)
		return;

	head = &node->children.list_nodes;

	for (p = head->next; p != head; p = p->next)
	{
		struct HashNode *hash_node = p->container;

		TrieNodeExit(hash_node->val);
	}

	HashExit(&node->children);
}

void TrieNodeInit(struct TrieNode *node, char c, struct TrieNode *parent)
{
	node->c = c;

	HashInit(&node->children, FuncIntToInt, FuncIntCompare);
	node->parent = parent;

	ListNodeInit(&node->hook, node);
}

