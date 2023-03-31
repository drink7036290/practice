
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../lib/helpers.h"

#include "../lib/hash.h"

struct WordInfo
{
	char *word;
	int index;

	struct ListNode hook;
};

int WordInfoMatchAfterShift(struct WordInfo *word_info)
{
	++word_info->index;
	return (strlen(word_info->word) - 1 == word_info->index);
}

void WordInfoInit(struct WordInfo *word_info, char *word)
{
	word_info->word = word;
	word_info->index = 0;

	ListNodeInit(&word_info->hook, word_info);
}

void DumpWords(struct Hash *dict_hash)
{
	struct ListNode *list_nodes = &dict_hash->list_nodes;
	struct ListNode *head = list_nodes, *p = NULL;

	printf("=====\n");
	for (p = head->next; p != head; p = p->next)
	{
		struct HashNode *hash_node = p->container;
		char *key = hash_node->key;

		struct ListNode *list_words = hash_node->val;
		struct ListNode *head2 = list_words, *p2 = NULL;

		printf("%s ->", key);
		for (p2 = head2->next; p2 != head2; p2 = p2->next)
		{
			struct WordInfo *word_info = p2->container;
			printf(" (%s, %d)", word_info->word, word_info->index);
		}
		printf("\n");
	}
	printf("=====\n");
}

void InsertWordInfoToHash(struct Hash *dict_hash, struct WordInfo *word_info)
{
	char *key = calloc(1 + 1, sizeof(char));

	struct ListNode *list_words = NULL;	
	struct HashNode *hash_node = NULL;

	*key = word_info->word[word_info->index]; 

	hash_node = HashGet(dict_hash, key);

	if (hash_node == NULL)
	{
		list_words = calloc(1, sizeof(struct ListNode));
		ListInit(list_words);

		HashInsert(dict_hash, key, list_words);
	}
	else
	{
		free(key);
		list_words = hash_node->val;
	}

	ListAddTail(list_words, &word_info->hook);
}

void InsertWordToHash(struct Hash *dict_hash, char *word)
{
	struct WordInfo *word_info = calloc(1, sizeof(struct WordInfo));

	WordInfoInit(word_info, word);

	InsertWordInfoToHash(dict_hash, word_info);
}

void UpdateWord(struct Hash *dict_hash, struct WordInfo *word_info, int *max_len, char* *returned_word)
{
	if (WordInfoMatchAfterShift(word_info))
	{
		if (*max_len < word_info->index)
		{
			*max_len = word_info->index;
			*returned_word = word_info->word;
		}

		return;
	}

	InsertWordInfoToHash(dict_hash, word_info);
}

void UpdateWords(struct Hash *dict_hash, struct HashNode *hash_node, int *max_len, char* *returned_word)
{
	struct ListNode *list_words = hash_node->val;
	struct ListNode *head = list_words, *p = NULL, *n = NULL;

	HashDeleteNode(dict_hash, hash_node);

	for (p = head->next, n = p->next; p != head; p = n, n = p->next)
	{
		struct WordInfo *word_info = p->container;

		ListNodeDelete(&word_info->hook);

		UpdateWord(dict_hash, word_info, max_len, returned_word);
	}

	DumpWords(dict_hash);
}

char* FindLongestWord(struct Hash *dict_hash, char *s)
{
	char *c = calloc(1 + 1, sizeof(char));

	int max_len = 0;
	char* returned_word = NULL;

	while ((*c = *s) != '\0')
	{
		struct HashNode *hash_node = HashGet(dict_hash, c);

		if (NULL != hash_node)
			UpdateWords(dict_hash, hash_node, &max_len, &returned_word);

		++s;
	}

	free(c);

	return returned_word;
}

void BuildDictHash(struct Hash *dict_hash, char **dict, int num_dict)
{
	int i = 0;

	for (i = 0; i < num_dict; ++i)
		InsertWordToHash(dict_hash, dict[i]);

	DumpWords(dict_hash);
}

char* LongestWord(char *s, char **dict, int num_dict)
{
	struct Hash dict_hash;

	HashInit(&dict_hash, HashStrToIndex, FuncStrCompare);

	BuildDictHash(&dict_hash, dict, num_dict);

	return FindLongestWord(&dict_hash, s);
}

int main(int argc, char *argv[])
{
#define kNumDict 5
	char S[] = "abppplee";
	char* D[kNumDict] = {"able", "ale", "apple", "bale", "kangaroo"};

	printf("%s\n", LongestWord(S, D, kNumDict));

	return 0;
}

