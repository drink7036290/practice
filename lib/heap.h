
#ifndef HEAP_H_
#define HEAP_H_

#include "heap_node.h"

struct Heap
{
	enum HeapType type;

	FuncObjCompare func_obj_compare;
	FuncObjUpdateHeapNode func_obj_update_heap_node;

	struct HeapNode *root;
	struct HeapNode *last_parent;

	struct ListNode list_nodes;
	int size;
};

void* HeapTop(struct Heap *heap);

void* HeapPopTail(struct Heap *heap);
void* HeapPop(struct Heap *heap); // extract

struct HeapNode* HeapInsert(struct Heap *heap, void *obj);

void HeapDump(struct Heap *heap, FuncObjDump func_obj_dump);

void HeapInit(struct Heap *heap, enum HeapType type, FuncObjCompare func_obj_compare);

// ============================================================

void MakeHeap(int *nums, int num_size, enum HeapType type);
void HeapArraySort(int *nums, int num_size, enum HeapType type);

#endif
