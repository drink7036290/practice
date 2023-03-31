
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "lib/helpers.h"

#include "lib/list_node.h"

typedef int (*FuncHasNext)(void *container);
typedef void* (*FuncGetNext)(void *container);

struct Iterator
{
	void *container;

	FuncHasNext func_has_next;
	FuncGetNext func_get_next;

	struct ListNode hook;
};

int IteratorHasNext(struct Iterator *iterator)
{
	return iterator->func_has_next(iterator->container);
}

void* IteratorGetNext(struct Iterator *iterator)
{
	if (! IteratorHasNext(iterator))
		return 0;

	return iterator->func_get_next(iterator->container);
}

void IteratorInit(struct Iterator *iterator, void *container, FuncHasNext func_has_next, FuncGetNext func_get_next)
{
	iterator->container = container;

	iterator->func_has_next = func_has_next;
	iterator->func_get_next = func_get_next;

	ListNodeInit(&iterator->hook, iterator);
}

struct IF
{
	struct ListNode list_iterator;
};

int IFHasNext(struct IF *_if)
{
	return (! ListIsEmpty(&_if->list_iterator));
}

void* IFGetNext(struct IF *_if)
{
	void *ret = NULL;

	struct ListNode *list_node = NULL;
	struct Iterator *iterator = NULL;

	if (! IFHasNext(_if))
		return NULL;
	
	list_node = ListPopFront(&_if->list_iterator);
	iterator = list_node->container;
		
	ret = IteratorGetNext(iterator);

	if (IteratorHasNext(iterator))
		ListAddTail(&_if->list_iterator, &iterator->hook);

	return ret;
}

void IFInit(struct IF *_if, struct Iterator *array, int array_size)
{
	int i = 0;

	ListInit(&_if->list_iterator);

	for (i = 0; i < array_size; ++i)
	{
		if (! IteratorHasNext(&array[i]))
			continue;

		ListAddTail(&_if->list_iterator, &array[i].hook);
	}
}

struct IntArray
{
	int *nums;
	int num_size;
	int index;
};

int FuncIntArrayHasNext(void *container)
{
	struct IntArray *int_array = container;

	return int_array->index < int_array->num_size;
}

void* FuncIntArrayGetNext(void *container)
{
	struct IntArray *int_array = container;

	if (! FuncIntArrayHasNext(container))
		return NULL;

	return &int_array->nums[int_array->index++];
}

void IntArrayInit(struct IntArray *int_array, int *nums, int num_size)
{
	int_array->nums = nums;
	int_array->num_size = num_size;

	int_array->index = 0;
}

#define kNumIterators 3

void TestIF(void)
{
	int nums_A[] = {1, 2, 3};
	int nums_B[] = {4, 5};
	int nums_C[] = {6, 7, 8, 9};

	struct IntArray int_array_A, int_array_B, int_array_C;
	struct Iterator *iterators = calloc(3, sizeof(struct Iterator));

	struct IF _if;

	IntArrayInit(&int_array_A, nums_A, sizeof(nums_A) / sizeof(nums_A[0]));
	IntArrayInit(&int_array_B, nums_B, sizeof(nums_B) / sizeof(nums_B[0]));
	IntArrayInit(&int_array_C, nums_C, sizeof(nums_C) / sizeof(nums_C[0]));

	IteratorInit(&iterators[0], &int_array_A, FuncIntArrayHasNext, FuncIntArrayGetNext);
	IteratorInit(&iterators[1], &int_array_B, FuncIntArrayHasNext, FuncIntArrayGetNext);
	IteratorInit(&iterators[2], &int_array_C, FuncIntArrayHasNext, FuncIntArrayGetNext);

	IFInit(&_if, iterators, kNumIterators);

	while (IFHasNext(&_if))
	{
		int *val = IFGetNext(&_if);
		printf(" %d", *val);
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	TestIF();

	return 0;
}

