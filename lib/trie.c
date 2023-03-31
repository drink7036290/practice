
#include "trie.h"
#include "helpers.h"

#include "stdio.h"

char* TrieGet(struct Trie *trie, char *str, enum TrieMatchState match_state)
{
	char *c = str;
	struct TrieNode *node = &trie->root, *last = NULL;

	if (StrIsEmpty(str))
		return NULL;

	while (*c != '\0')
	{
		if ((node = TrieNodeGet(node, *c)) == NULL)
			return NULL;

		if (TrieNodeIsEnd(node))
		{
			last = node;

			if (match_state == TrieMatchFirstEnds)
				return TrieNodePathStr(last);
		}

		++c;
	}

	if (match_state == TrieMatchCurrStr)
		return str;

	// TrieMatchExactEnds
	if (last == NULL)
		return NULL;

	return TrieNodePathStr(last);
}

void TrieAddStr(struct Trie *trie, char *str)
{
	char *c = str;
	struct TrieNode *node = &trie->root;

	if (StrIsEmpty(str))
		return;

	while (*c != '\0')
	{
		node = TrieNodeAdd(node, *c);
		++c;
	}

	TrieNodeMarkEnd(node);
}

void TrieAddStrs(struct Trie *trie, char **strs, int strs_num)
{
	int i = 0;

	for (i = 0; i < strs_num; ++i)
		TrieAddStr(trie, strs[i]);
}

void TrieDump(struct Trie *trie) // depth first
{
	printf("%s [BEGIN]\n", __FUNCTION__);
	TrieNodeDump(&trie->root);
	printf("%s [END]\n", __FUNCTION__);
}

void TrieExit(struct Trie *trie)
{
	TrieNodeExit(&trie->root);
}

void TrieInit(struct Trie *trie)
{
	TrieNodeInit(&trie->root, '\0', NULL); // \0 for PathStr ending
}
