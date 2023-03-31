
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list_node.h"

void ListSpliceHelper(struct ListNode *prev, struct ListNode *next, struct ListNode *list2)
{
	struct ListNode *first = list2->next, *last = list2->prev;

	prev->next = first;
	first->prev = prev;

	last->next = next;
	next->prev = last;
}

// append list2 to list1’s front
void ListSpliceFront(struct ListNode *list1, struct ListNode *list2)
{
	if (ListIsEmpty(list2))
		return;

	ListSpliceHelper(list1, list1->next, list2);
}

// append list2 to list1’s tail
void ListSpliceTail(struct ListNode *list1, struct ListNode *list2)
{
	if (ListIsEmpty(list2))
		return;

	ListSpliceHelper(list1->prev, list1, list2);
}

void ListNodeConnectSelf(struct ListNode *node)
{
	node->next = node;
	node->prev = node;
}

void* ListNodePrevContainer(struct ListNode *node)
{
        return node->prev->container;
}

void* ListNodeNextContainer(struct ListNode *node)
{
	return node->next->container;
}

int ListIsEmpty(struct ListNode *head)
{
	return (head->next == head);
}

int ListSize(struct ListNode *head)
{
	struct ListNode *p = NULL;
	int count = 0;

	for (p = head->next; p != head; p = p->next)
		++count;

	return count;
}

struct ListNode* ListGetRangeMiddle(struct ListNode *start, struct ListNode *end)
{
	struct ListNode *slow = start, *fast = start;

	while ((fast != end) && (fast->next != end))
	{
		slow = slow->next;
		fast = fast->next->next;
	}

	return slow;
}

void ListMergeSortMerge(struct ListNode *start, struct ListNode *mid, struct ListNode *end
			, enum SortType sort_type, FuncObjCompare func_obj_compare, FuncObjDump func_obj_dump)
{
	struct ListNode *before = start->prev;
	struct ListNode *p1 = start, *p2 = mid->next;

	// [TRICKEY !!]
	struct ListNode *after_end = end->next;
	while (p2 != after_end)
	{
		int pick_node2 = ((sort_type == SortTypeNonDecreasing) == (func_obj_compare(p1->container, p2->container) > 0));

/*		printf("pick_node2 %d p1 ", pick_node2);
		func_obj_dump(p1->container);
		printf("p2 ");
		func_obj_dump(p2->container);
*/
		if (pick_node2)
		{
			struct ListNode *tmp = p2; // [IMPORTANT !!]
			p2 = p2->next;

			ListNodeDelete(tmp);
			ListAddFront(before, tmp);
		}
		else
		{
			if (p1 == mid) // [IMPORTANT !!]
				break;

			p1 = p1->next;
		}

		before = before->next; // [IMPORTANT !!]
	}

//	for (p1 = org_before->next; p1 != org_after_end; p1 = p1->next)
//		func_obj_dump(p1->container);
}

void ListMergeSortRange(struct ListNode *start, struct ListNode *end
			, enum SortType sort_type, FuncObjCompare func_obj_compare, FuncObjDump func_obj_dump)
{
	struct ListNode *mid = NULL, *after_mid = NULL;
	struct ListNode *before_start = NULL, *after_end = NULL;

	if (start == end)
	{
/*		printf("start ");
                func_obj_dump(start->container);
                printf("end ");
                func_obj_dump(end->container);

		printf("start == end\n");
*/		return;
	}

	mid = ListGetRangeMiddle(start, end);

	// [IMPORTANT !!] save first
	before_start = start->prev;
	after_mid = mid->next;

	ListMergeSortRange(start, mid, sort_type, func_obj_compare, func_obj_dump);

	// [IMPORTANT !!] save first
	mid = after_mid->prev; 
	after_end = end->next;

	ListMergeSortRange(after_mid, end, sort_type, func_obj_compare, func_obj_dump);

	ListMergeSortMerge(before_start->next, mid, after_end->prev, sort_type, func_obj_compare, func_obj_dump);
}

void ListMergeSort(struct ListNode *list, enum SortType sort_type, FuncObjCompare func_obj_compare, FuncObjDump func_obj_dump)
{
	struct ListNode *head = list;

	ListMergeSortRange(head->next, head->prev, sort_type, func_obj_compare, func_obj_dump);
}
/*
void ArrayMergeSortMerge(int *nums, int start, int mid, int end, enum SortType type, int *helper)
{
	int i = start, j = mid + 1, curr = start;

	for (i = start; i <= end; ++i)
		helper[i] = nums[i];

	i = start;

	while ((i <= mid) && (j <= end))
	{
		int pick_j = ((type == SortTypeNonDecreasing) == (nums[i] > nums[j]));

		if (pick_j)
			nums[curr++] = helper[j++];
		else
			nums[curr++] = helper[i++];
	}

	while (i <= mid)
		nums[curr++] = helper[i++];
}

void ArrayMergeSortRange(int *nums, int start, int end, enum SortType type, int *helper)
{
	int mid = -1;

	if (start == end)
		return;

	mid = ((start + end) >> 1);

	ArrayMergeSortRange(nums, start, mid, type, helper);
	ArrayMergeSortRange(nums, mid + 1, end, type, helper);

	ArrayMergeSortMerge(nums, start, mid, end, type, helper);
}

void ArrayMergeSort(int *nums, int num_size, enum SortType type)
{
	int *helper = calloc(num_size, sizeof(int));

	ArrayMergeSortRange(nums, 0, num_size - 1, type, helper);
}
 */
void ArrayMergeSortMerge(void **objs, int start, int mid, int end
			, enum SortType sort_type, FuncObjCompare func_obj_compare, FuncObjDump func_obj_dump
			, void **helper)
{
	int i = 0, j = 0;

	for (i = start; i <= end; ++i)
		helper[i] = objs[i];

	i = start;
	j = mid + 1;

	while ((i <= mid) && (j <= end))
	{
		int pick_j = ((sort_type == SortTypeNonDecreasing) == (func_obj_compare(helper[i], helper[j]) > 0));

		if (pick_j)
		{
			objs[start] = helper[j];
			++j;
		}
		else
		{
			objs[start] = helper[i];
			++i;
		}

		++start;
	}

	while (i <= mid)
	{
		objs[start] = helper[i];
		++i;
		++start;
	}
}

void ArrayMergeSortRange(void **objs, int start, int end
			, enum SortType sort_type, FuncObjCompare func_obj_compare, FuncObjDump func_obj_dump
			, void **helper)
{
	int half = -1;

	if (start >= end)
		return;

	half = (start + end) / 2;

	ArrayMergeSortRange(objs, start, half, sort_type, func_obj_compare, func_obj_dump, helper);
	ArrayMergeSortRange(objs, half + 1, end, sort_type, func_obj_compare, func_obj_dump, helper);

	ArrayMergeSortMerge(objs, start, half, end, sort_type, func_obj_compare, func_obj_dump, helper);
}

void ArrayMergeSort(void **objs, int num_obj, enum SortType sort_type, FuncObjCompare func_obj_compare, FuncObjDump func_obj_dump)
{
	void **helper = calloc(num_obj, sizeof(int));

	ArrayMergeSortRange(objs, 0, num_obj - 1, sort_type, func_obj_compare, func_obj_dump, helper);
}

struct ListNode* ListPartitionRange(struct ListNode *start, struct ListNode *end
					, enum SortType sort_type, FuncObjCompare func_obj_compare, FuncObjDump func_obj_dump)
{
//	struct ListNode *before_start = start->prev, *after_end = end->next;
	struct ListNode *i = start->prev, *j = start;

	while (j != end)
	{
		struct ListNode *next = j->next;

		int pick_j = ((sort_type == SortTypeNonDecreasing) == (func_obj_compare(j->container, end->container) > 0));

//		printf("pick_j %d j ", pick_j);
//		func_obj_dump(j->container);

		if (! pick_j)
		{
			if (i->next != j)
			{
				ListNodeDelete(j);
				ListAddFront(i, j);
			}
			
			i = i->next;	
		}

		j = next;
	}

	ListNodeDelete(j);
	ListAddFront(i, j);
/*
	printf("=====after partition\n");
	for (i = before_start->next; i != after_end; i = i->next)
		func_obj_dump(i->container);
	printf("=====\n");	
*/
	return end;
}

void ListQuickSortRange(struct ListNode *start, struct ListNode *end
			, enum SortType sort_type, FuncObjCompare func_obj_compare, FuncObjDump func_obj_dump)
{
	struct ListNode *before_start = NULL, *after_end = NULL;
	struct ListNode *part = NULL;
/*
	printf("start ");
	func_obj_dump(start->container);
	printf("end ");
	func_obj_dump(end->container);
	printf("=====\n");
*/
	if (start == end)
		return;

	// [IMPORTANT !!] save first
	before_start = start->prev;
	after_end = end->next;

	part = ListPartitionRange(start, end, sort_type, func_obj_compare, func_obj_dump);

	if (before_start->next != part) // [IMPORTANT !!]
		ListQuickSortRange(before_start->next, part->prev, sort_type, func_obj_compare, func_obj_dump);

	if (part != after_end->prev) // [IMPORTANT !!]
		ListQuickSortRange(part->next, after_end->prev, sort_type, func_obj_compare, func_obj_dump);

}

void ListQuickSort(struct ListNode *list, enum SortType sort_type, FuncObjCompare func_obj_compare, FuncObjDump func_obj_dump)
{
	struct ListNode *head = list;
	
	ListQuickSortRange(head->next, head->prev, sort_type, func_obj_compare, func_obj_dump);
}

int ArrayPartitionRange(int *nums, int start, int end
			, enum SortType sort_type, FuncObjCompare func_obj_compare, FuncObjDump func_obj_dump)
{
	int i = start - 1, j = start;

	while (j != end)
	{
		int pick_j = ((sort_type == SortTypeNonDecreasing) == (func_obj_compare(&nums[j], &nums[end]) > 0));

		if (! pick_j)
		{
			++i;

			if (i != j)
				Swap(&nums[i], &nums[j]);
		}

		++j;
	}

	++i;

	if (i != end)
		Swap(&nums[i], &nums[end]);

	return i;
}

void ArrayQuickSortRange(int *nums, int start, int end
			, enum SortType sort_type, FuncObjCompare func_obj_compare, FuncObjDump func_obj_dump)
{
	int part = -1;

	if (start >= end)
		return;

	part = ArrayPartitionRange(nums, start, end, sort_type, func_obj_compare, func_obj_dump);

	ArrayQuickSortRange(nums, start, part - 1, sort_type, func_obj_compare, func_obj_dump);
	ArrayQuickSortRange(nums, part + 1, end, sort_type, func_obj_compare, func_obj_dump);
}

void ArrayQuickSort(int *nums, int num_size, enum SortType sort_type, FuncObjCompare func_obj_compare, FuncObjDump func_obj_dump)
{
	ArrayQuickSortRange(nums, 0, num_size - 1, sort_type, func_obj_compare, func_obj_dump);
}

void ListNodeDelete(struct ListNode *node)
{
	node->prev->next = node->next;
	node->next->prev = node->prev;

	ListNodeConnectSelf(node);
}

void ListNodeInsert(struct ListNode *node, struct ListNode *prev, struct ListNode *next)
{
	next->prev = node;
	node->next = next;

	prev->next = node;
	node->prev = prev;
}

struct ListNode* ListPopTail(struct ListNode *head)
{
	if (! ListIsEmpty(head))
	{
		struct ListNode *node = head->prev;
		ListNodeDelete(node);

		return node;
	}

	return NULL;
}

struct ListNode* ListPopFront(struct ListNode *head)
{
	if (! ListIsEmpty(head))
	{
		struct ListNode *node = head->next;
		ListNodeDelete(node);

		return node;
	}

	return NULL;
}

void ListAddTail(struct ListNode *head, struct ListNode *node)
{
	ListNodeInsert(node, head->prev, head);
}

void ListAddFront(struct ListNode *head, struct ListNode *node)
{
	ListNodeInsert(node, head, head->next);
}

void ListNodeInit(struct ListNode *node, void *container)
{
	node->container = container;

	ListNodeConnectSelf(node);
}

void ListInit(struct ListNode *node)
{
	ListNodeInit(node, NULL); // NULL container if it's a head not a hook
}

