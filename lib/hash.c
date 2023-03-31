
#include "hash.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int HashKeyToIndex(struct Hash *hash, void *key)
{
	int val = hash->func_obj_to_int(key);
	int ret = val % kHashBucketSize;

	return (ret < 0) ? ret + kHashBucketSize : ret ;
}

int HashKeyCompare(void *obj1, void *obj2)
{
	struct HashNode *hash_node1 = obj1, *hash_node2 = obj2;
	struct Hash *hash = hash_node1->container;

	return hash->func_obj_compare(hash_node1->key, hash_node2->key);
}

void HashSort(struct Hash *hash, enum SortType sort_type, FuncObjDump func_obj_dump)
{
//	ListMergeSort(&hash->list_nodes, sort_type, HashKeyCompare, func_obj_dump);
	ListQuickSort(&hash->list_nodes, sort_type, HashKeyCompare, func_obj_dump);
}

struct HashNode* HashGetOrCreate(struct Hash *hash, void *key, int create)
{
	int index = HashKeyToIndex(hash, key);

	struct ListNode *head = &hash->buckets[index], *p = NULL;

	for (p = head->next; p != head; p = p->next)
	{
		struct HashNode *hash_node = p->container;

		if (! hash->func_obj_compare(hash_node->key, key)) // equal
			return hash_node;
	}

	if (create)
	{
		struct HashNode *hash_node = calloc(1, sizeof(struct HashNode));
		HashNodeInit(hash_node, key, hash);

		ListAddTail(&hash->buckets[index], &hash_node->bucket_hook);

		ListAddTail(&hash->list_nodes, &hash_node->total_hook);

		return hash_node;
	}

	return NULL;
}

void* HashGet(struct Hash *hash, void *key)
{
	struct HashNode *hash_node = HashGetOrCreate(hash, key, 0);

	if (hash_node == NULL)
		return NULL;

	return hash_node->val;
}

void HashDeleteNode(struct Hash *hash, struct HashNode *hash_node)
{
	if (hash_node == NULL)
		return;

	ListNodeDelete(&hash_node->total_hook);

	ListNodeDelete(&hash_node->bucket_hook);
	
	free(hash_node);
}

void HashDelete(struct Hash *hash, void *key)
{
	HashDeleteNode(hash, HashGetOrCreate(hash, key, 0));
}

struct HashNode* HashInsert(struct Hash *hash, void *key, void *val)
{
	struct HashNode *hash_node = HashGetOrCreate(hash, key, 1);

	hash_node->val = val;

	return hash_node;
}

void HashExit(struct Hash *hash)
{
	struct ListNode *head = &hash->list_nodes, *p = NULL, *n = NULL;

	for (p = head->next, n = p->next; p != head; p = n, n = p->next)
	{
		struct HashNode *hash_node = p->container;

		ListNodeDelete(p);

		free(hash_node);
	}
}

void HashInit(struct Hash *hash, FuncObjToInt func_obj_to_int, FuncObjCompare func_obj_compare)
{
	int i = 0;

	for (i = 0; i < kHashBucketSize; ++i)
		ListInit(&hash->buckets[i]);
	
	ListInit(&hash->list_nodes);

	hash->func_obj_to_int = func_obj_to_int;
	hash->func_obj_compare = func_obj_compare;
}

