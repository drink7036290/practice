
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helpers.h"
#include "heap.h"

void* HeapTop(struct Heap *heap)
{
	if (heap->root != NULL)
		return heap->root->obj;

	return NULL;
}

void* HeapPopHelper(struct Heap *heap, int pop_tail)
{
	struct HeapNode *node = NULL;
	void *obj = NULL;

	if (ListIsEmpty(&heap->list_nodes))
		return NULL;

	node = ListNodePrevContainer(&heap->list_nodes);

	if (node == heap->root) // root only
	{
		heap->root = NULL;
		heap->last_parent = NULL;
	}
	else
	{
		if (! pop_tail)
			HeapNodeSwap(node, heap->root, heap->func_obj_update_heap_node);

		HeapNodeDisconnect(node, &heap->last_parent);

		if (! pop_tail)
			HeapNodeDown(heap->root, heap->func_obj_compare, heap->func_obj_update_heap_node);
	}

	--heap->size;
	ListNodeDelete(&node->hook);

	obj = node->obj;
	free(node);

	return obj;
}

void* HeapPopTail(struct Heap *heap)
{
	return HeapPopHelper(heap, 1);
}

void* HeapPop(struct Heap *heap)
{
	return HeapPopHelper(heap, 0);
}

struct HeapNode* HeapInsert(struct Heap *heap, void *obj)
{
	struct HeapNode *node = calloc(1, sizeof(struct HeapNode));

	HeapNodeInit(node, obj, heap->type);

	ListAddTail(&heap->list_nodes, &node->hook);
	++heap->size;

	if (heap->root == NULL)
	{
		heap->root = node;
		heap->last_parent = node;
	}
	else
		HeapNodeConnect(node, &heap->last_parent);

	HeapNodeUp(node, heap->func_obj_compare, heap->func_obj_update_heap_node);

	return node;
}

void HeapDump(struct Heap *heap, FuncObjDump func_obj_dump)
{
//	struct ListNode *head = &heap->list_nodes, *p = NULL;

	printf("(size %d)\n", heap->size);
	HeapNodeDump(heap->root, func_obj_dump, 0);
/*
	if (ListIsEmpty(head))
		printf("(list empty)\n");
	else
	{
		printf("(list)\n ");
		for (p = head->next; p != head; p = p->next)
		{
			struct HeapNode *node = p->container;
			//HeapNodeDump(node, func_obj_dump, 0); // recursive
			func_obj_dump(node->obj);
		}
		printf("\n");
	}
*/
}

void HeapInit(struct Heap *heap, enum HeapType type, FuncObjCompare func_obj_compare)
{
	heap->type = type;

	heap->func_obj_compare = func_obj_compare;
	heap->func_obj_update_heap_node = NULL;

	heap->root = NULL;
	heap->last_parent = NULL;

	ListInit(&heap->list_nodes);
	heap->size = 0;
}

// ============================================================

void MakeHeap(int *nums, int num_size, enum HeapType type)
{
	int half = num_size >> 1;
	int i = 0;
	
	for (i = half; i >= 0; --i)
		HeapArrayNodeDown(nums, num_size, i, type);
}

void HeapArraySort(int *nums, int num_size, enum HeapType type)
{
	int i = num_size - 1;

	MakeHeap(nums, num_size, type);

	while (i > 0)
	{
		Swap(&nums[i], &nums[0]);

		HeapArrayNodeDown(nums, i, 0, type); // array size become 0 ~ i-1, i.e., i

		--i;
	}
}

