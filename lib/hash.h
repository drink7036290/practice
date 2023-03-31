
#ifndef HASH_H_
#define HASH_H_

#include "hash_node.h"

#define kHashBucketSize 1024

struct Hash
{
	struct ListNode buckets[kHashBucketSize];
	struct ListNode list_nodes;

	FuncObjToInt func_obj_to_int;
	FuncObjCompare func_obj_compare;
};

void HashSort(struct Hash *hash, enum SortType sort_type, FuncObjDump func_obj_dump);

void* HashGet(struct Hash *hash, void *key);

void HashDeleteNode(struct Hash *hash, struct HashNode *hash_node);
void HashDelete(struct Hash *hash, void *key);

struct HashNode* HashInsert(struct Hash *hash, void *key, void *val);

void HashExit(struct Hash *hash);
void HashInit(struct Hash *hash, FuncObjToInt func_obj_to_int, FuncObjCompare func_obj_compare);

#endif
