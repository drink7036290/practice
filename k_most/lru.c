
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../lib/helpers.h"

#include "../lib/hash.h"
#include "../lib/heap.h"

struct DataInfo
{
	int address;
	char *str;

	long long timestamp;
	struct HeapNode *heap_node;
};

void FuncDataInfoUpdateHeapNode(void *obj, struct HeapNode *heap_node)
{
	struct DataInfo *data_info = obj;

	data_info->heap_node = heap_node;
}

int FuncDataInfoCompare(void *obj1, void *obj2)
{
	struct DataInfo *data_info1 = obj1;
	struct DataInfo *data_info2 = obj2;

	long long int *val1 = &data_info1->timestamp;
	long long int *val2 = &data_info2->timestamp;

	if (*val1 < *val2)
		return -1;

	if (*val1 == *val2)
		return 0;

	return 1;
}

void DataInfoDump(struct DataInfo *data_info)
{
	printf(" addr %d str %s time %lld", data_info->address, data_info->str, data_info->timestamp);
}

void FuncDataInfoDump(void *obj)
{
	DataInfoDump(obj);
}

void DataInfoInit(struct DataInfo *data_info, int address, char *str, long long timestamp)
{
	data_info->address = address;
	data_info->str = str;

	data_info->timestamp = timestamp;
	data_info->heap_node = NULL;
}

struct LRU
{
	struct Heap heap;
	long long timestamp;
	int max_size;

	struct Hash hash;
};

void HashDump(struct Hash *hash)
{
	struct ListNode *head = &hash->list_nodes, *p = NULL;

	printf("%s\n", __FUNCTION__);

	for (p = head->next; p != head; p = p->next)
	{
		struct HashNode *hash_node = p->container; 
		struct DataInfo *data_info = hash_node->val;

		DataInfoDump(data_info);
		printf("\n");
	}
}

void LRUDump(struct LRU *lru)
{
	printf("max_size %d\n", lru->max_size);
	HeapDump(&lru->heap, FuncDataInfoDump);
//	HashDump(&lru->hash);
	printf("\n");
}

void LRUInsert(struct LRU *lru, int address, char *str)
{
	struct DataInfo *data_info = NULL;

	if (ListSize(&lru->heap.list_nodes) == lru->max_size)
	{
		data_info = HeapPop(&lru->heap); // extract min
		HashDelete(&lru->hash, &data_info->address);
		free(data_info);
	}

	if ((data_info = HashGet(&lru->hash, &address)) == NULL)
	{
		int *key = calloc(1, sizeof(int));
		struct DataInfo *data_info = calloc(1, sizeof(struct DataInfo));

		*key = address;
		DataInfoInit(data_info, address, str, ++lru->timestamp);

		HashInsert(&lru->hash, key, data_info);
		data_info->heap_node = HeapInsert(&lru->heap, data_info);
	}
	else
	{
		data_info->timestamp = ++lru->timestamp;
		HeapNodeDown(data_info->heap_node, FuncDataInfoCompare, FuncDataInfoUpdateHeapNode);
	}
}

void LRUInit(struct LRU *lru, int max_size)
{
	HeapInit(&lru->heap, kMinHeap, FuncDataInfoCompare);
	lru->heap.func_obj_update_heap_node = FuncDataInfoUpdateHeapNode;

	lru->timestamp = 0;
	lru->max_size = max_size;

	HashInit(&lru->hash, FuncIntToInt, FuncIntCompare);
}

void TestLRU(int num, int max_size)
{
	struct LRU lru;

	LRUInit(&lru, max_size);

	srand(time(NULL));

	while (num > 0)
	{
		char *str = calloc(5, sizeof(char));
		sprintf(str, "%d", rand() % 10000);

		LRUInsert(&lru, rand() % 100, str);

		LRUDump(&lru);

		--num;
	}
}

int main(int argc, char *argv[])
{
	TestLRU(atoi(argv[1]), atoi(argv[2]));

	return 0;
}

