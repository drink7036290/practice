
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "lib/helpers.h"

#include "lib/hash.h"
#include "lib/heap.h"

struct ValInfo
{
	int val;
	int index;

	struct HeapNode *min_node;
	struct HeapNode *max_node;
};

void FuncValInfoUpdateMinNode(void *obj, struct HeapNode *heap_node)
{
	struct ValInfo *val_info = obj;

	val_info->min_node = heap_node;
}

void FuncValInfoUpdateMaxNode(void *obj, struct HeapNode *heap_node)
{
	struct ValInfo *val_info = obj;

	val_info->max_node = heap_node;
}

int FuncValInfoCompare(void *obj1, void *obj2)
{
	struct ValInfo *val_info1 = obj1;
	struct ValInfo *val_info2 = obj2;

	return FuncIntCompare(&val_info1->index, &val_info2->index);
}

void ValInfoDump(struct ValInfo *val_info)
{
	printf(" val %d index %d", val_info->val, val_info->index);
}

void FuncValInfoDump(void *obj)
{
	ValInfoDump(obj);
}

void ValInfoInit(struct ValInfo *val_info, int val)
{
	val_info->val = val;
	val_info->index = -1;

	val_info->min_node = NULL;
	val_info->max_node = NULL;
}

struct Range
{
	int start;
	int end; // included
};

void RangeDump(struct Range *pair)
{
	printf("start %d end %d len %d\n", pair->start, pair->end, pair->end - pair->start + 1);
}

void RangeInit(struct Range *pair, int start, int end)
{
	pair->start = start;
	pair->end = end;
}

void PrepareHash(struct Hash *hash, int *shorter, int num_shorter)
{
	int i = 0;

	for (i = 0; i < num_shorter; ++i)
	{
		struct ValInfo *val_info = calloc(1, sizeof(struct ValInfo));

		ValInfoInit(val_info, shorter[i]);

		HashInsert(hash, &shorter[i], val_info);
	}
}

void UpdateRange(struct Heap *max_heap, struct Heap *min_heap, struct Range* pair)
{
	struct ValInfo *val_info_max = HeapTop(max_heap);
	struct ValInfo *val_info_min = HeapTop(min_heap);

	int len = val_info_max->index - val_info_min->index + 1;

	if ((len < pair->end - pair->start + 1) || (pair->start == -1))
	{
		RangeInit(pair, val_info_min->index, val_info_max->index);
		RangeDump(pair);
	}
}

struct Range* GetRange(struct Hash *hash, struct Heap *max_heap, struct Heap *min_heap
		, int *longer, int num_longer)
{
	int i = 0, cn_untouched = ListSize(&hash->list_nodes); // num_shorter

	struct Range *pair = calloc(1, sizeof(struct Range));
	RangeInit(pair, -1, -1);

	for (i = 0; i < num_longer; ++i)
	{
		struct ValInfo *val_info = HashGet(hash, &longer[i]);

		if (val_info == NULL)
			continue;
		
		if (val_info->index == -1)
			--cn_untouched;

		val_info->index = i;

		if (val_info->min_node == NULL)
			val_info->min_node = HeapInsert(min_heap, val_info);
		else
			HeapNodeDown(val_info->min_node
					, FuncValInfoCompare, FuncValInfoUpdateMinNode);

		printf("\nmin\n");
		HeapDump(min_heap, FuncValInfoDump);

		if (val_info->max_node == NULL)
			val_info->max_node = HeapInsert(max_heap, val_info);
		else
			HeapNodeUp(val_info->max_node
					, FuncValInfoCompare, FuncValInfoUpdateMaxNode);

		printf("\nmax\n");
		HeapDump(max_heap, FuncValInfoDump);

		if (cn_untouched == 0)
			UpdateRange(max_heap, min_heap, pair);
	}

	return pair;
}

int CheckShorter(struct Hash *hash)
{
	struct ListNode *head = &hash->list_nodes, *p = NULL;

	for (p = head->next; p != head; p = p->next)
	{
		struct HashNode *hash_node = p->container;

		struct ValInfo *val_info = hash_node->val;

		if (val_info->index == -1)
			return 0;
	}

	return 1;
}

void HashDump(struct Hash *hash)
{
	struct ListNode *head = &hash->list_nodes, *p = NULL;

	for (p = head->next; p != head; p = p->next)
	{
		struct HashNode *hash_node = p->container;

		struct ValInfo *val_info = hash_node->val;

		ValInfoDump(val_info);
	}
	printf("\n");
}

struct Range* GetSuperSequenceLen(int *shorter, int num_shorter, int *longer, int num_longer)
{
	struct Hash hash;
	struct Heap max_heap, min_heap;
	struct Range *pair = NULL;

	HashInit(&hash, FuncIntToInt, FuncIntCompare);

	PrepareHash(&hash, shorter, num_shorter);

	HashDump(&hash);

	HeapInit(&max_heap, kMaxHeap, FuncValInfoCompare);
	max_heap.func_obj_update_heap_node = FuncValInfoUpdateMaxNode;

	HeapInit(&min_heap, kMinHeap, FuncValInfoCompare);
	min_heap.func_obj_update_heap_node = FuncValInfoUpdateMinNode;

	pair = GetRange(&hash, &max_heap, &min_heap, longer, num_longer);

	if (! CheckShorter(&hash))
	{
		free(pair);
		return NULL;
	}

	return pair;
}

int main(int argc, char *argv[])
{
	int shorter[] = {1, 5, 9};
	int longer[] = {7, 5, 9, 9, 2, 1, 3, 5, 7, 9, 1, 1, 5, 8, 8, 9, 7};

	struct Range *pair = GetSuperSequenceLen(shorter, sizeof(shorter)/sizeof(shorter[0])
						, longer, sizeof(longer)/sizeof(longer[0]));

	if (pair == NULL)
		printf("ERROR\n");
	else
	{
		RangeDump(pair);
		free(pair);
	}

	return 0;
}

