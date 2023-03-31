
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../lib/helpers.h"

#include "../lib/hash.h"

struct WordInfo
{
	char *str;

	struct ListNode hook;
};

void WordInfoDump(struct WordInfo *word_info)
{
	printf(" %s", word_info->str);
}

void WordInfoInit(struct WordInfo *word_info, char *word)
{
	word_info->str = word;

	ListNodeInit(&word_info->hook, word_info);
}

// ===============================================================

struct WordMap
{
	struct Hash word_to_wild; // word to list_wild
	struct Hash wild_to_word; // wild to list_word
};

void AddToList(char *w, struct ListNode *list)
{
	struct WordInfo *info = calloc(1, sizeof(struct WordInfo));
	WordInfoInit(info, w);

	ListAddTail(list, &info->hook);
}

struct ListNode* AddToHash(struct Hash *hash, char *word)
{
	struct ListNode *list = NULL;

	struct HashNode *hash_node = HashGet(hash, word);

	if (hash_node == NULL)
	{
		list = calloc(1, sizeof(struct ListNode));
		ListInit(list);

		HashInsert(hash, word, list);
	}
	else
		list = hash_node->val;

	return list;
}

void HashWild(struct Hash *word_to_wild, char *word, struct ListNode *list_wild)
{
	int len = strlen(word), i = 0;

	for (i = 0; i < len; ++i)
	{
		struct ListNode *list_word = NULL;

		char *wild = StrConcat(NULL, word);
		wild[i] = '*';
//		printf("wild %s\n", wild);

		AddToList(wild, list_wild);

		list_word = AddToHash(word_to_wild, wild);
		AddToList(word, list_word);
	}
}

void HashWord(struct WordMap *word_map, char *word)
{
	struct ListNode *list_wild = AddToHash(&word_map->word_to_wild, word);

	HashWild(&word_map->word_to_wild, word, list_wild);
}

void HashWords(struct WordMap *word_map, char **words, int num_word)
{
	int i = 0;

	for (i = 0; i < num_word; ++i)
		HashWord(word_map, words[i]);
}

void HashDump(struct Hash *hash)
{
	struct ListNode *head = &hash->list_nodes, *p = NULL;

	for (p = head->next; p != head; p = p->next)
	{
		struct HashNode *hash_node = p->container;

		struct ListNode *list_words = hash_node->val;
		struct ListNode *head2 = list_words, *p2 = NULL;

		char *key = hash_node->key;
		printf("%s ->", key);

		for (p2 = head2->next; p2 != head2; p2 = p2->next)
		{
			struct WordInfo *word_info = p2->container;
			WordInfoDump(word_info);
		}

		printf("\n");
	}
}

void WordMapDump(struct WordMap *word_map)
{
	HashDump(&word_map->word_to_wild);
	HashDump(&word_map->wild_to_word);
}

void WordMapInit(struct WordMap *word_map, char **words, int num_word)
{
	HashInit(&word_map->word_to_wild, HashStrToIndex, FuncStrCompare);
	HashInit(&word_map->wild_to_word, HashStrToIndex, FuncStrCompare);

	HashWords(word_map, words, num_word);
}

struct PathInfo
{
	char *str;
	struct PathInfo *previous;	

	struct ListNode hook_BFS;
	struct ListNode hook_path;
};

void PathInfoInit(struct PathInfo *path_info, char *word, struct PathInfo *previous)
{
	path_info->str = word;
	path_info->previous = previous;

	ListNodeInit(&path_info->hook_BFS, path_info);
	ListNodeInit(&path_info->hook_path, path_info);
}

struct BFSData
{
	struct WordMap *word_map;

	struct Hash hash_visited;
	struct ListNode list_path_info_BFS;

	struct BFSData *the_other;
};

int BFSDataIsVisited(struct BFSData *bfs_data, char *word)
{
	struct HashNode *hash_node = HashGet(&bfs_data->hash_visited, word);

	return (hash_node != NULL);
}

void BFSDataMarkVisited(struct BFSData *bfs_data, char *word, struct PathInfo *path_info)
{
	if (BFSDataIsVisited(bfs_data, word))
		return;

	HashInsert(&bfs_data->hash_visited, word, path_info);
}

int BFSDataIsDone(struct BFSData *bfs_data)
{
	return ListIsEmpty(&bfs_data->list_path_info_BFS);
}

void BFSDataDumpQueue(struct BFSData *bfs_data)
{
	struct ListNode *head = &bfs_data->list_path_info_BFS, *p = NULL;

	printf("%s\n", __FUNCTION__);
	for (p = head->next; p != head; p = p->next)
	{
		struct PathInfo *path_info = p->container;
		printf(" %s", path_info->str);
	}
	printf("\n");
}

char* BFSDataGetWords(struct BFSData *bfs_data, char *wild, struct PathInfo *previous_path_info)
{
	struct HashNode *hash_node = HashGet(&bfs_data->word_map->word_to_wild, wild);
	struct ListNode *list_word = NULL, *head = NULL, *p = NULL;

	if (hash_node == NULL)
		return 0;

	list_word = hash_node->val;
	head = list_word;

	for (p = head->next; p != head; p = p->next)
	{
		struct WordInfo *word_info = p->container;
		struct PathInfo *path_info = NULL;

		if (BFSDataIsVisited(bfs_data, word_info->str))
			continue;

		printf("word %s previous %s\n", word_info->str, previous_path_info->str);

		path_info = calloc(1, sizeof(struct PathInfo));
		PathInfoInit(path_info, word_info->str, previous_path_info);

		BFSDataMarkVisited(bfs_data, word_info->str, path_info);

		if (BFSDataIsVisited(bfs_data->the_other, word_info->str))
		{
			printf("found %s\n", word_info->str);

			return word_info->str;
		}

		printf("add to BFS %s\n", path_info->str);
		ListAddTail(&bfs_data->list_path_info_BFS, &path_info->hook_BFS);
	}

	BFSDataDumpQueue(bfs_data);

	return NULL;
}

char* BFSDataGetWilds(struct BFSData *bfs_data, struct PathInfo *path_info)
{
	struct HashNode *hash_node = HashGet(&bfs_data->word_map->word_to_wild, path_info->str);
	struct ListNode *list_wild = NULL, *head = NULL, *p = NULL;

	if (hash_node == NULL)
		return NULL;

	list_wild = hash_node->val;
	head = list_wild;

	for (p = head->next; p != head; p = p->next)
	{
		struct WordInfo *wild_info = p->container;
		char *meet = NULL;
	
		printf("wild %s\n", wild_info->str);
	
		if ((meet = BFSDataGetWords(bfs_data, wild_info->str, path_info)) != NULL)
			return meet;
	}

	return NULL;
}

char* BFSDataSearch(struct BFSData *bfs_data)
{
	struct ListNode *head = &bfs_data->list_path_info_BFS, *p = NULL, *n = NULL;
	int count = ListSize(head);

	for (p = head->next, n = p->next; p != head; p = n, n = p->next)
	{
		struct PathInfo *path_info = p->container;
		char *meet = NULL;

		ListNodeDelete(p);

//		printf("%s \"%s\"\n", __FUNCTION__, path_info->str);

		if ((meet = BFSDataGetWilds(bfs_data, path_info)) != NULL)
			return meet;

		if (--count == 0) // search one level only
			break;
	}
	
	return NULL;
}

void BFSDataInit(struct BFSData *bfs_data, struct WordMap *word_map, char *start_word, struct BFSData *bfs_data_other)
{
	struct PathInfo *path_info = calloc(1, sizeof(struct PathInfo));
	PathInfoInit(path_info, start_word, NULL);

	bfs_data->word_map = word_map;

	ListInit(&bfs_data->list_path_info_BFS); // act as BFS queue
	ListAddTail(&bfs_data->list_path_info_BFS, &path_info->hook_BFS);

	HashInit(&bfs_data->hash_visited, HashStrToIndex, FuncStrCompare);
	BFSDataMarkVisited(bfs_data, start_word, path_info);

	bfs_data->the_other = bfs_data_other;
}

void MergePath(struct BFSData *bfs_forward, struct BFSData *bfs_backward, char *meet, struct ListNode *list_merged)
{
	struct HashNode *hash_node1 = HashGet(&bfs_forward->hash_visited, meet);
	struct HashNode *hash_node2 = HashGet(&bfs_backward->hash_visited, meet);

	struct PathInfo *path_info1 = hash_node1->val;
	struct PathInfo *path_info2 = hash_node2->val;

	struct PathInfo *path_info = path_info1;

	while (path_info != NULL)
	{
		struct WordInfo *word_info = calloc(1, sizeof(struct WordInfo));
		WordInfoInit(word_info, path_info->str);

		ListAddFront(list_merged, &word_info->hook);

		path_info = path_info->previous;
	}
	
	if (! ListIsEmpty(list_merged))
		ListNodeDelete(list_merged->prev);

	path_info = path_info2;

	while (path_info != NULL)
        {
                struct WordInfo *word_info = calloc(1, sizeof(struct WordInfo));
                WordInfoInit(word_info, path_info->str);

                ListAddTail(list_merged, &word_info->hook);

                path_info = path_info->previous;
        }
}

void DumpMergedPath(struct ListNode *list)
{
	struct ListNode *head = list, *p = NULL;

	printf("%s\n", __FUNCTION__);
	for (p = head->next; p != head; p = p->next)
	{
		struct WordInfo *word_info = p->container;

		printf(" %s", word_info->str);
	}
	printf("\n");
}

void Trasform(char *start_word, char *end_word, char **words, int num_word)
{
	struct WordMap word_map;
	struct BFSData bfs_forward, bfs_backward;
	struct ListNode list_path;

	WordMapInit(&word_map, words, num_word);
	HashWord(&word_map, start_word);

	WordMapDump(&word_map);

	BFSDataInit(&bfs_forward, &word_map, start_word, &bfs_backward);
	BFSDataInit(&bfs_backward, &word_map, end_word, &bfs_forward);

	ListInit(&list_path);

	while ((! BFSDataIsDone(&bfs_forward)) && (! BFSDataIsDone(&bfs_backward)))
	{
		char *meet = NULL;

		printf("forward\n");
		meet = BFSDataSearch(&bfs_forward);

		if (meet == NULL)
		{
			printf("backward\n");
			meet = BFSDataSearch(&bfs_backward);
		}

		if (meet != NULL)
		{
			MergePath(&bfs_forward, &bfs_backward, meet, &list_path);
			break;
		}
	}

	DumpMergedPath(&list_path);
}

int main(int argc, char *argv[])
{
#define kNumWord 6
	char* words[kNumWord] = {"hot","dot","dog","lot","log","cog"};
//#define kNumWord 5
//	char* words[kNumWord] = {"hot","dot","dog","lot","log"};
	char start_word[] = "hit";
	char end_word[] = "cog";

	Trasform(start_word, end_word, words, kNumWord);

	return 0;
}

