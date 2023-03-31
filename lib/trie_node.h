
#ifndef TRIE_NODE_H_
#define TRIE_NODE_H_

#include "hash.h"

struct TrieNode
{
	char c; // '\0' means root

	struct Hash children; // char -> TrieNode, '\0' means string termination
	struct TrieNode *parent;

	struct ListNode hook;
};

int TrieNodeDepth(struct TrieNode *node);
char* TrieNodePathStr(struct TrieNode *node);

struct TrieNode* TrieNodeGet(struct TrieNode *node, char c);
struct TrieNode* TrieNodeAdd(struct TrieNode *node, char c);

int TrieNodeIsEnd(struct TrieNode *node);
void TrieNodeMarkEnd(struct TrieNode *node);

void TrieNodeDump(struct TrieNode *node);

void TrieNodeExit(struct TrieNode *node);
void TrieNodeInit(struct TrieNode *node, char c, struct TrieNode *parent);

#endif
