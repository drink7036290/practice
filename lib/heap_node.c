
#include <stdio.h>
#include <limits.h>
#include <string.h>

#include "../lib/helpers.h"
#include "heap_node.h"

enum HeapNodeDirection HeapNodeGetDirection(struct HeapNode *node, void *new_obj, FuncObjCompare func_obj_compare)
{
	int compare = func_obj_compare(node->obj, new_obj);

	if (! compare) // equal
		return kNodeStay;

	if ((node->type == kMinHeap) && (compare > 0)) // new smaller
		return kNodeUp;

	if ((node->type == kMaxHeap) && (compare < 0)) // new larger
		return kNodeUp;

	return kNodeDown;
}

void HeapNodeSwap(struct HeapNode *node1, struct HeapNode *node2, FuncObjUpdateHeapNode func_obj_update_heap_node)
{
	SwapObj(&node1->obj, &node2->obj);

	if (func_obj_update_heap_node == NULL)
		return;

	func_obj_update_heap_node(node1->obj, node1);
	func_obj_update_heap_node(node2->obj, node2);
}

void HeapNodeDown(struct HeapNode *node, FuncObjCompare func_obj_compare, FuncObjUpdateHeapNode func_obj_update_heap_node)
{
	while (node != NULL)
	{
		struct HeapNode *target = NULL;
		void *obj = node->obj;

		if (node->left != NULL)
		{
			if (HeapNodeGetDirection(node->left, obj, func_obj_compare) == kNodeDown)
			{
				target = node->left;
				obj = target->obj;
			}
		}

		if (node->right != NULL)
		{
			if (HeapNodeGetDirection(node->right, obj, func_obj_compare) == kNodeDown)
			{
				target = node->right;
				obj = target->obj;
			}
		}

		if (target != NULL)
			HeapNodeSwap(node, target, func_obj_update_heap_node);

		node = target;
	}
}

void HeapNodeUp(struct HeapNode *node, FuncObjCompare func_obj_compare, FuncObjUpdateHeapNode func_obj_update_heap_node)
{
	while (node->parent != NULL)
	{
		if (HeapNodeGetDirection(node->parent, node->obj, func_obj_compare) != kNodeUp)
			break;

		HeapNodeSwap(node, node->parent, func_obj_update_heap_node);
		node = node->parent;
	}
}

void HeapNodeDisconnect(struct HeapNode *child, struct HeapNode **last_parent)
{
	struct HeapNode *parent = child->parent;

	if (parent == NULL)
		return;

	if (parent->left == child)
		parent->left = NULL;
	else // right child
	{
		parent->right = NULL;
		*last_parent = parent;
	}

	child->parent = NULL;
}

void HeapNodeConnect(struct HeapNode *child, struct HeapNode **last_parent)
{
	struct HeapNode *parent = *last_parent;

	child->parent = parent;

	if (parent->left == NULL)
		parent->left = child;
	else // right == NULL
	{
		parent->right = child;
		*last_parent = ListNodeNextContainer(&parent->hook);
	}
}

void HeapNodeDump(struct HeapNode *node, FuncObjDump func_obj_dump, int shift) // depth first
{
	int i = 0;

	if (node == NULL)
		return;
/*
	printf("%s:%d child %p parent %p left %p right %p container %p\n", __FUNCTION__, __LINE__
		, node, node->parent, node->left, node->right, (struct HeapNode*)(node->hook.container));
*/
	for (i = 0; i < shift; ++i)
		printf(" ");

	func_obj_dump(node->obj);
	printf("\n");

	HeapNodeDump(node->left, func_obj_dump, shift + 2);
	HeapNodeDump(node->right, func_obj_dump, shift + 2);
}

void HeapNodeInit(struct HeapNode *node, void *obj, enum HeapType type)
{
	node->obj = obj;

	node->type = type;

	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;

	ListNodeInit(&node->hook, node);
}

// ============================================================

int ParentIndex(int i)
{
	if (i == 0)
	        return -1;

	return ((i - 1) >> 1);
}

int LeftIndex(int i)
{
	return (i << 1) + 1;
}

int RightIndex(int i)
{
	return (i << 1) + 2;
}

enum HeapNodeDirection HeapArrayNodeGetDirection(int from, int to, enum HeapType type)
{
	if (from == to)
		return kNodeStay;

	if ((type == kMinHeap) && (from > to))
		return kNodeUp;

	if ((type == kMaxHeap) && (from < to))
		return kNodeUp;

	return kNodeDown;
}

void HeapArrayNodeDown(int *nums, int num_size, int i, enum HeapType type)
{
	while (i < num_size)
	{
		int left = LeftIndex(i);
		int right = RightIndex(i);

		int target = num_size;
		int val = nums[i];

		if (left < num_size)
		{
			if (HeapArrayNodeGetDirection(nums[left], val, type) == kNodeDown)
			{
				target = left;
				val = nums[left];
			}
		}

		if (right < num_size)
		{
			if (HeapArrayNodeGetDirection(nums[right], val, type) == kNodeDown)
			{
				target = right;
				val = nums[right];
			}
		}

		if (target != num_size)
			Swap(&nums[target], &nums[i]);

		i = target;
	}
}

