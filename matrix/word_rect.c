
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../lib/helpers.h"

#include "../lib/trie.h"

struct WordInfo
{
	char *word;
	struct ListNode hook;
};

void WordInfoDump(struct WordInfo *word_info)
{
	printf(" %s", word_info->word);
}

void WordInfoInit(struct WordInfo *word_info, char *word)
{
	word_info->word = word;
	ListNodeInit(&word_info->hook, word_info);
}

struct WordGroup
{
	struct ListNode list_word_infos;
	struct Trie trie;
};

void WordGroupDump(struct WordGroup *word_group)
{
	struct ListNode *head = &word_group->list_word_infos, *p = NULL;

	printf("list_word_infos:");
	for (p = head->next; p != head; p = p->next)
	{
		struct WordInfo *word_info = p->container;
		WordInfoDump(word_info);
	}
	printf("\n");

//	TrieDump(&word_group->trie);
}

void WordGroupInsert(struct WordGroup *word_group, char *word)
{
	struct WordInfo *word_info = calloc(1, sizeof(struct WordInfo));

	WordInfoInit(word_info, word);

	ListAddTail(&word_group->list_word_infos, &word_info->hook);
	TrieAddStr(&word_group->trie, word);
}

void WordGroupInit(struct WordGroup *word_group)
{
	ListInit(&word_group->list_word_infos);
	TrieInit(&word_group->trie);
}

struct Rect
{
	int num_row;
	int num_col;
	char **strs;

	struct WordGroup *word_group_row;
	struct WordGroup *word_group_col;

	int word_square;
};

void RectDump(struct Rect *rect)
{
	int i = 0;

	printf("num row %d col %d\n", rect->num_row, rect->num_col);

//	WordGroupDump(rect->word_group_row);
//	WordGroupDump(rect->word_group_col);

	for (i = 0; i < rect->num_row; ++i)
		printf("%s\n", rect->strs[i]);
}

void RectInit(struct Rect *rect, int num_row, int num_col, struct WordGroup *word_group_row, struct WordGroup *word_group_col)
{
	rect->num_row = num_row;
	rect->num_col = num_col;
	rect->strs = calloc(num_row, sizeof(char*));

	rect->word_group_row = word_group_row;
	rect->word_group_col = word_group_col;

	rect->word_square = 0;
}

int RectCheckWordSquare(struct Rect *rect, int row)
{
	int i = 0, j = 0;

	if (! rect->word_square)
		return 1;
	
	for (i = 1; i <= row; ++i) // 1 ~ <=row
	{
		for (j = 0; j < i; ++j)
		{
			if (rect->strs[i][j] != rect->strs[j][i])
				return 0;
		}
	}

	return 1;
}

int RectCheckCols(struct Rect *rect, int row)
{
	int i = 0, j = 0;

	for (j = 0; j < rect->num_col; ++j)
	{
		char *col_str = NULL;

		for (i = 0; i <= row; ++i) // <=
			col_str = StrConcatChar(col_str, rect->strs[i][j]);

//		printf("col %d str %s\n", j, col_str);

		if (NULL == TrieGet(&rect->word_group_col->trie, col_str, TrieMatchCurrStr))
		{
//			printf("\"%s\" not in trie\n", col_str);
			return 0;
		}
	}

	return 1;
}

int RectBuildRow(struct Rect *rect, int row)
{
	struct ListNode *list_word_infos_row = &rect->word_group_row->list_word_infos;
	struct ListNode *head = list_word_infos_row, *p = NULL;
	
	if (row == rect->num_row)
	{
		RectDump(rect);
		return 1;
	}

	for (p = head->next; p != head; p = p->next)
	{
		struct WordInfo *word_info = p->container;

		rect->strs[row] = word_info->word;

//		printf("row %d str %s\n", row, rect->strs[row]);

		if (! RectCheckWordSquare(rect, row))
                        continue;

		if (! RectCheckCols(rect, row))
			continue;

		if (RectBuildRow(rect, row + 1))
		{
//			return 1;
		}
	}

	return 0;
}

int RectBuild(struct Rect *rect)
{
	return RectBuildRow(rect, 0);
}

void HashNodeDump(struct HashNode *hash_node)
{
	int *key = hash_node->key;
//	struct WordGroup *word_group = hash_node->val;

	printf("(key) %d \n", *key);
//	WordGroupDump(word_group);
}

void FuncHashNodeDump(void *obj)
{
	HashNodeDump(obj);
}

void HashDump(struct Hash *hash_word_group)
{
	struct ListNode *head = &hash_word_group->list_nodes, *p = NULL;

	for (p = head->next; p != head; p = p->next)
		HashNodeDump(p->container);
}

void SplitWordsByLen(struct Hash *hash, char **words, int num_word)
{
	int i = 0;

	for (i = 0; i < num_word; ++i)
	{
		int len = strlen(words[i]);

		struct WordGroup *word_group = NULL;

		if (len == 0)
			continue;

		if ((word_group = HashGet(hash, &len)) == NULL)
		{
			int *key = calloc(1, sizeof(int));
			word_group = calloc(1, sizeof(struct WordGroup));

			*key = len;
			WordGroupInit(word_group);

			HashInsert(hash, key, word_group);
		}

		WordGroupInsert(word_group, words[i]);
	}

//	HashDump(hash);
}

int CollectMaxMinLen(struct Hash *hash, int *max_len, int *min_len)
{
	struct ListNode *head = &hash->list_nodes, *p = NULL;
	struct HashNode *hash_node = NULL;
	int *key = NULL;

	if (! ListIsEmpty(head))
	{
		p = head->next;
		hash_node = p->container;
		key = hash_node->key;

		*max_len = *key;

		p = head->prev;
		hash_node = p->container;
		key = hash_node->key;

		*min_len = *key;
	}

	return ((*max_len > 0) && (*min_len > 0));
}

struct Rect* MaxRect(char **words, int num_word)
{
	struct Hash hash_word_group;
	struct Rect *rect = calloc(1, sizeof(struct Rect));

	int max_len = INT_MIN, min_len = INT_MAX;
	int max_area = INT_MIN, min_area = INT_MAX;

	int area = 0;
	int word_square = 1;

	HashInit(&hash_word_group, FuncIntToInt, FuncIntCompare);

	SplitWordsByLen(&hash_word_group, words, num_word);

	HashSort(&hash_word_group, SortTypeDecreasing, FuncHashNodeDump);

	printf("=====after sorting\n");
	HashDump(&hash_word_group);

	if (! CollectMaxMinLen(&hash_word_group, &max_len, &min_len))
		return NULL;

	max_area = max_len * max_len;
	min_area = min_len * min_len;

	printf("len max %d min %d / area max %d min %d\n", max_len, min_len, max_area, min_area);

	for (area = max_area; area >= min_area; --area)
	{
		struct ListNode *head = &hash_word_group.list_nodes, *p = NULL;

		for (p = head->next; p != head; p = p->next) // from max_len to min_len
		{
			struct HashNode *hash_node = p->container;
			int *key = hash_node->key;
			int num_col = *key, num_row = 0;

			struct WordGroup *word_group_col = NULL;

			if (area % num_col != 0)
				continue;

			num_row = area / num_col;

			if (word_square && (num_row != num_col))
				continue;

			// num_row : word_group_col
			// num_col : word_group_row
			if ((word_group_col = HashGet(&hash_word_group, &num_row)) == NULL)
				continue;

			RectInit(rect, num_row, num_col, hash_node->val, word_group_col);
			rect->word_square = word_square;

			if (RectBuild(rect))
			{
//				return rect;
			}
		}
	}

	return NULL;
}

int main(int argc, char *argv[])
{
//	struct Rect *rect = NULL;

//#define kNumWord 1
//	char* words[kNumWord] = {""};

//#define kNumWord 1
//	char* words[kNumWord] = {"a"};

//#define kNumWord 2
//	char* words[kNumWord] = {"a", "ab"};

//#define kNumWord 5
//	char* words[kNumWord] = {"a", "ab", "abc", "bcd", "cde"};

#define kNumWord 5
	char* words[kNumWord] = {"", "a", "ab", "aa", "bb"};

//#define kNumWord 4
//	char* words[kNumWord] = {"a", "ab", "abc", "abcd"};

//#define kNumWord 4
//	char* words[kNumWord] = {"THAT", "HASH", "ASIA", "FOOL"};

//#define kNumWord 6
//	char* words[kNumWord] = {"AREA", "BALL", "DEAR", "LADY", "LEAD", "YARD"};

//#define kNumWord 5
//	char* words[kNumWord] = {"a", "abc", "ab", "abcde", "abcd"};

	MaxRect(words, kNumWord);
/*	if ((rect = MaxRect(words, kNumWord)) != NULL)
	{
		printf("rect found!\n");
		RectDump(rect);
	}
*/
	return 0;
}

