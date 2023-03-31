
#ifndef LIST_NODE_H_
#define LIST_NODE_H_

#include "helpers.h"

struct ListNode
{
	void *container;

	struct ListNode *prev;
	struct ListNode *next;
};

// append list2 to list1’s front
void ListSpliceFront(struct ListNode *list1, struct ListNode *list2);

// append list2 to list1’s tail
void ListSpliceTail(struct ListNode *list1, struct ListNode *list2);

void* ListNodePrevContainer(struct ListNode *node);
void* ListNodeNextContainer(struct ListNode *node);

int ListIsEmpty(struct ListNode *head);
int ListSize(struct ListNode *head);

struct ListNode* ListGetRangeMiddle(struct ListNode *start, struct ListNode *end);

void ListMergeSort(struct ListNode *list, enum SortType sort_type, FuncObjCompare func_obj_compare, FuncObjDump func_obj_dump);

void ArrayMergeSort(void **nums, int num_size, enum SortType sort_type, FuncObjCompare func_obj_compare, FuncObjDump func_obj_dump);

void ListQuickSort(struct ListNode *list, enum SortType sort_type, FuncObjCompare func_obj_compare, FuncObjDump func_obj_dump);

void ArrayQuickSort(int *nums, int num_size, enum SortType sort_type, FuncObjCompare func_obj_compare, FuncObjDump func_obj_dump);

void ListNodeDelete(struct ListNode *node);
void ListNodeInsert(struct ListNode *node, struct ListNode *prev, struct ListNode *next);

struct ListNode* ListPopTail(struct ListNode *head);
struct ListNode* ListPopFront(struct ListNode *head);

void ListAddTail(struct ListNode *head, struct ListNode *node);
void ListAddFront(struct ListNode *head, struct ListNode *node);

void ListNodeInit(struct ListNode *node, void *container);
void ListInit(struct ListNode *node);

#endif
