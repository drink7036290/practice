
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../lib/helpers.h"

#include "../lib/trie.h"

char** PrepareDigitToChar(int num_digit)
{
	char **digit_to_char = calloc(num_digit, sizeof(char*));
	int max_len = 4;
	int i = 0;

	for (i = 0; i < num_digit; ++i)
		digit_to_char[i] = calloc(max_len + 1, sizeof(char));

	digit_to_char[2] = "abc";
	digit_to_char[3] = "def";
	digit_to_char[4] = "ghi";
	digit_to_char[5] = "jkl";
	digit_to_char[6] = "mno";	
	digit_to_char[7] = "pqrs";
	digit_to_char[8] = "tuv";
	digit_to_char[9] = "wxyz";

	return digit_to_char;
}

void TrieNodeCollectChildren(struct TrieNode *trie_node, struct ListNode *list_next, char *str)
{
	char *c = str;

	while (*c != '\0')
	{
		struct TrieNode *child_node = TrieNodeGet(trie_node, *c);

		if (child_node != NULL)
			ListAddTail(list_next, &child_node->hook);

		++c;
	}
}

void CollectChildren(struct ListNode *list_curr, struct ListNode *list_next, char *str)
{
	struct ListNode *head = list_curr, *p = NULL, *n = NULL;

	for (p = head->next, n = p->next; p != head; p = n, n = p->next)
	{
		struct TrieNode *trie_node = p->container;

		ListNodeDelete(p);

		TrieNodeCollectChildren(trie_node, list_next, str);
	}
}

struct StrInfo
{
	char *str;
	struct ListNode hook;
};

void StrInfoInit(struct StrInfo *str_info, char *str)
{
	str_info->str = str;

	ListNodeInit(&str_info->hook, str_info);
}

struct ListNode* ConvertTrieNodesToList(struct ListNode *list_curr)
{
	struct ListNode *head = list_curr, *p = NULL, *n = NULL;
	struct ListNode *list_result = calloc(1, sizeof(struct ListNode));

	if (ListIsEmpty(list_curr))
		return list_result;

	ListInit(list_result);

	for (p = head->next, n = p->next; p != head; p = n, n = p->next)
	{
		struct TrieNode *trie_node = p->container;
		struct StrInfo *str_info = NULL;

		ListNodeDelete(p);

		if (! TrieNodeIsEnd(trie_node))
			continue;

		str_info = calloc(1, sizeof(struct StrInfo));
		StrInfoInit(str_info, TrieNodePathStr(trie_node)); // until root

		printf("valid %s\n", str_info->str);

		ListAddTail(list_result, &str_info->hook);
	}

	return list_result;
}

void DumpList(struct ListNode *list)
{
	struct ListNode *head = list, *p = NULL;

        for (p = head->next; p != head; p = p->next)
        {
                struct TrieNode *trie_node = p->container;
		printf(" %c", trie_node->c);
	}
	printf("\n");
}

struct ListNode* GetMatch(char *digits, struct Trie *trie)
{
	int num_digit = 10; // 0 ~ 9
	char **digit_to_char = PrepareDigitToChar(num_digit);

	char *d = digits;	

	struct ListNode list_curr, list_next;
	ListInit(&list_curr);
	ListInit(&list_next);

	ListAddTail(&list_curr, &trie->root.hook);	

	while (*d != '\0')
	{
		CollectChildren(&list_curr, &list_next, digit_to_char[(*d) - '0']);

		if (ListIsEmpty(&list_next))
			return NULL;

		ListSpliceTail(&list_curr, &list_next);
		ListInit(&list_next);

		++d;
	}

	return ConvertTrieNodesToList(&list_curr);
}

void TestMatch(void)
{
	struct Trie trie;

	char *digits = calloc(4, sizeof(char));
	
	char *valid_strs[] = {
		  "tree"
		, "used"
		, "like"
	};

	int i = 0, num_valid = sizeof(valid_strs)/sizeof(valid_strs[0]);

	TrieInit(&trie);

	for (i = 0; i < num_valid; ++i)
		TrieAddStr(&trie, valid_strs[i]);

	TrieDump(&trie);

	sprintf(digits, "%d", /*rand() % 10000*/8733);
	printf("digits %s\n", digits);
	
	GetMatch(digits, &trie);
}

int main(int argc, char *argv[])
{
	TestMatch();

	return 0;
}

