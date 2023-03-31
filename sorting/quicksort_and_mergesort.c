
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include "../lib/helpers.h"

#include "../lib/list_node.h"

struct Item
{
	int num;

	struct ListNode hook;
};

void FuncItemDump(void *obj)
{
	struct Item *item = obj;

	printf(" %d\n", item->num);
}

int FuncItemCompare(void *obj1, void *obj2)
{
	struct Item *item1 = obj1, *item2 = obj2;

	return item1->num - item2->num;
}

void ItemInit(struct Item *item, int num)
{
	item->num = num;
	ListNodeInit(&item->hook, item);
}

void ListDump(struct ListNode *list)
{
	struct ListNode *head = list, *p = NULL;

	printf("===%s", __FUNCTION__);
	for (p = head->next; p != head; p = p->next)
	{
		struct Item *item = p->container;
		printf(" %d", item->num);
	}
	printf("===\n");
}

void TestQuickSort(int num)
{
	struct ListNode list;

	ListInit(&list);

	srand(time(NULL));

	while (num > 0)
	{
		struct Item *item = calloc(1, sizeof(struct Item));
		ItemInit(item, rand() % 100);

		ListAddTail(&list, &item->hook);

		--num;
	}

	printf("before sort\n");
	ListDump(&list);

	printf("quick sort\n");
	ListQuickSort(&list, SortTypeNonDecreasing, FuncItemCompare, FuncItemDump);
	ListDump(&list);

	ListQuickSort(&list, SortTypeDecreasing, FuncItemCompare, FuncItemDump);
	ListDump(&list);
}

void DumpArray(int *nums, int num_size)
{
	int i = 0;

	for (i = 0; i < num_size; ++i)
		printf(" %d", nums[i]);
	printf("\n");
}

void TestArrayQuickSort(int num)
{
	int *nums = calloc(num, sizeof(int));
	int i = 0;

	srand(time(NULL));

	for (i = 0; i < num; ++i)
		nums[i] = rand() % 100;

	printf("before sort\n");
	DumpArray(nums, num);
	
	printf("%s\n", __FUNCTION__);
	ArrayQuickSort(nums, num, SortTypeNonDecreasing, FuncIntCompare, FuncIntDump);
	DumpArray(nums, num);

	ArrayQuickSort(nums, num, SortTypeDecreasing, FuncIntCompare, FuncIntDump);
	DumpArray(nums, num);
}

void TestMergeSort(int num)
{
	struct ListNode list;

	ListInit(&list);

	srand(time(NULL));

	while (num > 0)
	{
		struct Item *item = calloc(1, sizeof(struct Item));
		ItemInit(item, rand() % 100);

		ListAddTail(&list, &item->hook);

		--num;
	}

	printf("before sort\n");
	ListDump(&list);

	printf("Merge sort\n");
	ListMergeSort(&list, SortTypeNonDecreasing, FuncItemCompare, FuncItemDump);
	ListDump(&list);

	ListMergeSort(&list, SortTypeDecreasing, FuncItemCompare, FuncItemDump);
	ListDump(&list);
}

void TestArrayMergeSort(int num)
{
	int *nums = calloc(num, sizeof(int));
	int i = 0;

	srand(time(NULL));

	for (i = 0; i < num; ++i)
		nums[i] = rand() % 100;

	printf("before sort\n");
	DumpArray(nums, num);
	
	printf("%s\n", __FUNCTION__);
	ArrayMergeSort(nums, num, SortTypeNonDecreasing, FuncIntCompare, FuncIntDump);
	DumpArray(nums, num);

	ArrayMergeSort(nums, num, SortTypeDecreasing, FuncIntCompare, FuncIntDump);
	DumpArray(nums, num);
}

int main(int argc, char *argv[])
{
	TestQuickSort(atoi(argv[1]));
	TestArrayQuickSort(atoi(argv[1]));

	TestMergeSort(atoi(argv[1]));
	TestArrayMergeSort(atoi(argv[1]));

	return 0;
}

