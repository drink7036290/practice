
#ifndef HASH_NODE_H_
#define HASH_NODE_H_

#include "list_node.h"

struct HashNode
{
	void *key;
	void *val;

	struct ListNode bucket_hook;
	struct ListNode total_hook;

	void *container;
};

void HashNodeInit(struct HashNode *hash_node, void *key, void *container);

#endif
