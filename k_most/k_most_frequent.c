
#include <stdio.h>
#include <stdlib.h>

#include "../lib/hash.h"
#include "../lib/heap.h"

struct WordInfo
{
	char *word;
	int freq;

	struct HeapNode *heap_node; // NULL if not in heap
	struct ListNode hook; // in final returned list
};

void WordInfoDump(struct WordInfo *info)
{
	printf("word %s freq %d\n", info->word, info->freq);
}

void WordInfoInit(struct WordInfo *info, char *word)
{
	info->word = word;
	info->freq = 0;

	info->heap_node = NULL;
	ListNodeInit(&info->hook, info);
}

struct WordInfo* PushToHash(struct Hash *hash, char *word)
{
	struct HashNode *hash_node = NULL;
	struct WordInfo *info = NULL;

	if ((hash_node = HashGet(hash, word)) == NULL)
	{
		info = malloc(1 * sizeof(struct WordInfo));

		WordInfoInit(info, word);
		hash_node = HashInsert(hash, word, info);		
	}

	info = hash_node->val;
	++info->freq;

	return info;
}

void PushToHeap(struct Heap *heap, struct WordInfo* info, int k)
{
	if (info->heap_node != NULL) // already in heap
	{
		HeapNodeSetVal(info->heap_node, info->freq);
		return;
	}

	if ((info->freq > HeapTop(heap)) || (heap->size < k))
		info->heap_node = HeapInsert(heap, info->freq, info);

	if (heap->size > k)
	{
		struct HeapNode *heap_node = HeapPop(heap);
		info = heap_node->user_data;
		info->heap_node = NULL;
	}
}

void HeapToList(struct Heap *heap, struct ListNode *list_return)
{
	struct ListNode *head = &heap->list_nodes, *p = NULL;

	for (p = head->next; p != head; p = p->next)
	{
		struct HeapNode *heap_node = p->container;
		struct WordInfo *info = heap_node->user_data;

		ListAddTail(list_return, &info->hook);
	}

	head = list_return;

	for (p = head->next; p != head; p = p->next)
	{
		struct WordInfo *info = p->container;
		WordInfoDump(info);
	}
}

struct ListNode* GetKMostFrequent(char **words, int words_size, int k)
{
	struct Hash hash;
	struct Heap heap;
	struct ListNode *list_return = NULL;

	int i = 0;

	HashInit(&hash, HashStrToIndex, FuncStrCompare);
	HeapInit(&heap, kMinHeap); // min heap

	for (i = 0; i < words_size; ++i)
	{
		struct WordInfo *info = PushToHash(&hash, words[i]);
		PushToHeap(&heap, info, k);
	}

	list_return = malloc(1 * sizeof(struct ListNode));
	ListInit(list_return);

	HeapToList(&heap, list_return);

	// SortList(list_return, fn_compare);

	return list_return;
}


int main(int argc, char *argv[])
{
#define kWordsSize 6
	char *words[kWordsSize] = {"i", "love", "leetcode", "i", "love", "coding"};
	int k = 2;
	
/*#define kWordsSize 10
	char *words[kWordsSize] = {"the","day","is","sunny","the","the","the","sunny","is","is"};
	int k = 4;
*/
	GetKMostFrequent(words, kWordsSize, k);

	return 0;
}

