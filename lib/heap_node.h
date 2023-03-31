
#ifndef HEAP_NODE_H_
#define HEAP_NODE_H_

#include "list_node.h"

enum HeapType
{
	  kMinHeap = 0
	, kMaxHeap
};

enum HeapNodeDirection
{
	  kNodeUp = 0
	, kNodeDown
	, kNodeStay
};

struct HeapNode
{
	void *obj;

	enum HeapType type;

	struct HeapNode *left;
	struct HeapNode *right;
	struct HeapNode *parent;

	struct ListNode hook;
};

typedef void (*FuncObjUpdateHeapNode)(void *obj, struct HeapNode *heap_node);

void HeapNodeSwap(struct HeapNode *node1, struct HeapNode *node2, FuncObjUpdateHeapNode func_obj_update_heap_node);

void HeapNodeDown(struct HeapNode *node, FuncObjCompare func_obj_compare, FuncObjUpdateHeapNode func_obj_update_heap_node);
void HeapNodeUp(struct HeapNode *node, FuncObjCompare func_obj_compare, FuncObjUpdateHeapNode func_obj_update_heap_node);

void HeapNodeDisconnect(struct HeapNode *child, struct HeapNode **last_parent);
void HeapNodeConnect(struct HeapNode *child, struct HeapNode **last_parent);

void HeapNodeDump(struct HeapNode *node, FuncObjDump func_obj_dump, int shift);

void HeapNodeInit(struct HeapNode *node, void *obj, enum HeapType type);

// ============================================================

void HeapArrayNodeDown(int *nums, int num_size, int i, enum HeapType type);

#endif
