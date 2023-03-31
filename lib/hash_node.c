
#include "hash_node.h"

#include "string.h"

void HashNodeInit(struct HashNode *hash_node, void *key, void *container)
{
	hash_node->key = key;

	hash_node->val = NULL;

	ListNodeInit(&hash_node->bucket_hook, hash_node);
	ListNodeInit(&hash_node->total_hook, hash_node);

	hash_node->container = container;
}

