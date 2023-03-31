
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../lib/helpers.h"

#include "../lib/graph.h"

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

void HashWild(struct Hash *wild_to_word, char *word, struct ListNode *list_wild)
{
	int len = strlen(word), i = 0;

	for (i = 0; i < len; ++i)
	{
		struct ListNode *list_word = NULL;

		char *wild = StrConcat(NULL, word);
		wild[i] = '*';
//		printf("wild %s\n", wild);

		AddToList(wild, list_wild);

		list_word = AddToHash(wild_to_word, wild);
		AddToList(word, list_word);
	}
}

void HashWord(struct WordMap *word_map, char *word)
{
	struct ListNode *list_wild = AddToHash(&word_map->word_to_wild, word);

	HashWild(&word_map->wild_to_word, word, list_wild);
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
	HashInit(&word_map->word_to_wild, FuncStrToInt, FuncStrCompare);
	HashInit(&word_map->wild_to_word, FuncStrToInt, FuncStrCompare);

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

void AddToGraph(struct Graph *graph, char *word, char *wild, struct Hash *wild_to_word)
{
	struct ListNode *list_word = HashGet(wild_to_word, wild);
	struct ListNode *head2 = list_word, *p2 = NULL;

	for (p2 = head2->next; p2 != head2; p2 = p2->next)
	{
		struct WordInfo *word_info = p2->container;

		if (! strcmp(word, word_info->str))
			continue;

		GraphAddEdge(graph, word, word_info->str);
		GraphAddEdge(graph, word_info->str, word);
	}
}

void PrepareGraph(struct Graph *graph, struct WordMap *word_map)
{
	struct ListNode *head = &word_map->word_to_wild.list_nodes, *p = NULL;

	for (p = head->next; p != head; p = p->next)
	{
		struct HashNode *hash_node = p->container;

		struct ListNode *list_wild = hash_node->val;
		struct ListNode *head2 = list_wild, *p2 = NULL;

		char *word = hash_node->key;

		for (p2 = head2->next; p2 != head2; p2 = p2->next)
		{
			struct WordInfo *word_info = p2->container;

			AddToGraph(graph, word, word_info->str, &word_map->wild_to_word);
		}
	}
}

struct MeetPair
{
	struct Vertex *vertex_from_start;
	struct Vertex *vertex_from_end;
};

void MeetPairDump(struct MeetPair *meet_pair)
{
	printf("%s from %s\n", (char*)meet_pair->vertex_from_start->obj, (char*)meet_pair->vertex_from_start->set->obj);
	printf("%s from %s\n", (char*)meet_pair->vertex_from_end->obj, (char*)meet_pair->vertex_from_end->set->obj);
}

void MeetPairInit(struct MeetPair *meet_pair, struct Vertex *vertex_from_start, struct Vertex *vertex_from_end)
{
	meet_pair->vertex_from_start = vertex_from_start;
	meet_pair->vertex_from_end = vertex_from_end;
}

struct MeetPair* BiDirBFS(struct Graph *graph, char *start_word, char *end_word)
{
	struct Vertex *vertex_start = HashGet(&graph->hash_vertex, start_word);
	struct Vertex *vertex_end = HashGet(&graph->hash_vertex, end_word);

	struct ListNode list_BFS;

	if ((vertex_start == NULL) || (vertex_end == NULL))
		return NULL;

	ListInit(&list_BFS);

	vertex_start->state = 1;
	ListAddTail(&list_BFS, &vertex_start->hook_DFS);
	
	vertex_end->state = 2;
	ListAddTail(&list_BFS, &vertex_end->hook_DFS);

	while (! ListIsEmpty(&list_BFS))
	{
		struct ListNode *head = NULL, *p = ListPopFront(&list_BFS);
		struct Vertex *vertex = p->container;

//		FuncStrDump(vertex->obj);

		head = &vertex->hash_edge.list_nodes;
		for (p = head->next; p != head; p = p->next)
		{
			struct Edge *edge = p->container;
			struct Vertex *next_vertex = HashGet(&graph->hash_vertex, edge->obj);

			if (next_vertex->state == vertex->set->state)
				continue;

			if (next_vertex->state == 0)
			{
				next_vertex->state = vertex->set->state;
				next_vertex->vertex_from = vertex;
				next_vertex->set = vertex->set;
			}
			else
			{
				struct MeetPair *meet_pair = calloc(1, sizeof(struct MeetPair));

				if (! strcmp((char*)vertex->set->obj, start_word))
					MeetPairInit(meet_pair, vertex, next_vertex);
				else
					MeetPairInit(meet_pair, next_vertex, vertex);

				return meet_pair;
			}
			
			printf("next %s from %s set %s\n", (char*)next_vertex->obj, (char*)vertex->obj, (char*)vertex->set->obj);

			ListAddTail(&list_BFS, &next_vertex->hook_DFS);
		}
	}

	return NULL;
}

void MergePath(struct MeetPair *meet_pair, struct ListNode *list_merged)
{
	struct Vertex *vertex_from_start = meet_pair->vertex_from_start;
	struct Vertex *vertex_from_end = meet_pair->vertex_from_end;

	while (vertex_from_start != NULL)
	{
		ListNodeDelete(&vertex_from_start->hook_DFS);
		ListAddFront(list_merged, &vertex_from_start->hook_DFS);

		vertex_from_start = vertex_from_start->vertex_from;
	}

	while (vertex_from_end != NULL)
	{
		ListNodeDelete(&vertex_from_end->hook_DFS);
		ListAddTail(list_merged, &vertex_from_end->hook_DFS);

		vertex_from_end = vertex_from_end->vertex_from;
	}
}

void Trasform(char *start_word, char *end_word, char **words, int num_word)
{
	struct WordMap word_map;
	struct Graph graph;
	struct MeetPair *meet_pair = NULL;

	struct ListNode list_path;

	WordMapInit(&word_map, words, num_word);
	HashWord(&word_map, start_word);

	WordMapDump(&word_map);

	GraphInit(&graph, FuncStrToInt, FuncStrCompare);

	PrepareGraph(&graph, &word_map);

	GraphDump(&graph, FuncStrDump);

	printf("\n\n");

	if ((meet_pair = BiDirBFS(&graph, start_word, end_word)) == NULL)
	{
		printf("\n\nNo path\n");
		return;
	}
	
	printf("\n\nmid pair\n");
	MeetPairDump(meet_pair);
	
	ListInit(&list_path);

	MergePath(meet_pair, &list_path);

	DumpMergedPath(&list_path);
}

int main(int argc, char *argv[])
{
	char* words[] =
////		{"hot","dot","dog","lot","log","cog"};
//		{"hot","dot","dog","lot","log"};
//	char start_word[] = "hit";
//	char end_word[] = "cog";

		{"DAMP","LAMP","LIMP","LIME","LIKE"};
	char start_word[] = "DAMP";
	char end_word[] = "LIKE";

	Trasform(start_word, end_word, words, sizeof(words)/sizeof(words[0]));

	return 0;
}

