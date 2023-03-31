
#ifndef TRIE_H_
#define TRIE_H_

#include "trie_node.h"

enum TrieMatchState
{
	  TrieMatchCurrStr
	, TrieMatchFirstEnds
	, TrieMatchExactEnds
};

struct Trie
{
	struct TrieNode root;
};

char* TrieGet(struct Trie *trie, char *str, enum TrieMatchState match_state);
void TrieAddStr(struct Trie *trie, char *str);

void TrieAddStrs(struct Trie *trie, char **strs, int strs_num);

void TrieDump(struct Trie *trie);

void TrieExit(struct Trie *trie);
void TrieInit(struct Trie *trie);

#endif

