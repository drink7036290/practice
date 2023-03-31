
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include "../lib/heap.h"

struct ValInfo
{
	int val;
	struct ListNode hook;
};

void FuncValInfoDump(void *obj)
{
	struct ValInfo *val_info = obj;

	printf(" %d", val_info->val);
}

void ValInfoInit(struct ValInfo *val_info, int val)
{
	val_info->val = val;
	ListNodeInit(&val_info->hook, val_info);
}

void HeapSort(struct Heap *heap, struct ListNode *list_result)
{
	struct ValInfo *val_info = NULL;
	struct ListNode *head = list_result, *p = NULL;

	while ((val_info = HeapPop(heap)) != NULL)
		ListAddFront(list_result, &val_info->hook);

	for (p = head->next; p != head; p = p->next)
	{
		val_info = p->container;
		printf(" %d", val_info->val);
	}
	printf("\n");
}

void TestHeapSort(int num)
{
	int cn = num;

	struct Heap max_heap, min_heap;
	struct ListNode list_result_max, list_result_min;

	HeapInit(&max_heap, kMaxHeap, FuncIntCompare);
	HeapInit(&min_heap, kMinHeap, FuncIntCompare);

	srand(time(NULL));

	while (cn > 0)
	{
		int val = rand() % 100;
		
		struct ValInfo *val_info = calloc(1, sizeof(struct ValInfo));
		ValInfoInit(val_info, val);

		HeapInsert(&max_heap, val_info);

		val_info = calloc(1, sizeof(struct ValInfo));
		ValInfoInit(val_info, val);

		HeapInsert(&min_heap, val_info);

		--cn;
	}

	printf("before sorting\n");
	HeapDump(&max_heap, FuncValInfoDump);
	HeapDump(&min_heap, FuncValInfoDump);

	printf("after sorting\n");
	ListInit(&list_result_max);
	ListInit(&list_result_min);

	HeapSort(&max_heap, &list_result_max);
	HeapSort(&min_heap, &list_result_min);
}

void DumpArray(int *nums, int num_size)
{
	int i = 0;

//	printf("DumpArray\n");
	for (i = 0; i < num_size; ++i)
		printf(" %d", nums[i]);
	printf("\n");
}

void TestHeapArraySort(int num_size)
{
	int *nums = calloc(num_size, sizeof(int));
	int i = 0;

	srand(time(NULL));
	
	for (i = 0; i < num_size; ++i)
		nums[i] = rand() % 100;

//	printf("Array init\n");
//	DumpArray(nums, num_size);

//	MakeHeap(nums, num_size, kMaxHeap);
//	printf("Make Max Heap\n");
//	DumpArray(nums, num_size);

	HeapArraySort(nums, num_size, kMaxHeap);
//	printf("sort\n");
	DumpArray(nums, num_size);

//	MakeHeap(nums, num_size, kMinHeap);
//	printf("Make Min Heap\n");
//	DumpArray(nums, num_size);	

	HeapArraySort(nums, num_size, kMinHeap);
//	printf("sort\n");
	DumpArray(nums, num_size);

	free(nums);
}

int main(int argc, char *argv[])
{
	int num_size = atoi(argv[1]);

	TestHeapSort(num_size);

	printf("\n");

	TestHeapArraySort(num_size);

	return 0;
}

