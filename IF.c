
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "lib/helpers.h"

typedef int (*FuncHasNext)(void *container);
typedef void* (*FuncGetNext)(void *container);

struct Iterator
{
	void *container;

	FuncHasNext func_has_next;
	FuncGetNext func_get_next;
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
}

struct IF
{
	struct Iterator *array;
	int array_size;
	int index;
};

int IFHasNext(struct IF *_if)
{
	int cn = _if->array_size;

	while (cn > 0)
	{
		struct Iterator *iterator = NULL;

		if (_if->index >= _if->array_size)
			_if->index -= _if->array_size;

		iterator = &_if->array[_if->index];

		if (IteratorHasNext(iterator))
			return 1;

		++_if->index;

		--cn;
	}

	return 0;
}

void* IFGetNext(struct IF *_if)
{
	struct Iterator *iterator = NULL;

	if (! IFHasNext(_if))
		return NULL;

	iterator = &_if->array[_if->index++];

	return IteratorGetNext(iterator);
}

void IFInit(struct IF *_if, struct Iterator *array, int array_size)
{
	_if->array = array;
	_if->array_size = array_size;

	_if->index = 0;
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

