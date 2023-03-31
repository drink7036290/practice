
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../lib/helpers.h"

#include "../lib/list_node.h"

struct Box
{
	int index;

	int height;
	int width;
	int depth;

	int pick;
	struct Box *prev;

	struct ListNode hook;
};

void BoxInit(struct Box *box, int index, int height, int width, int depth)
{
	box->index = index;

	box->height = height;
	box->width = width;
	box->depth = depth;

	box->pick = 0;
	box->prev = NULL;

	ListNodeInit(&box->hook, box);
}

void FuncBoxDump(void *obj)
{
	struct Box *box = obj;

	printf("index %d height %d width %d depth %d\n", box->index, box->height, box->width, box->depth);
}

int FuncHeightCompare(void *obj1, void *obj2)
{
	struct Box *box1 = obj1;
	struct Box *box2 = obj2;

	return FuncIntCompare(&box1->height, &box2->height);
}

int FuncWidthCompare(void *obj1, void *obj2)
{
	struct Box *box1 = obj1;
	struct Box *box2 = obj2;

	return FuncIntCompare(&box1->width, &box2->width);
}

int FuncDepthCompare(void *obj1, void *obj2)
{
	struct Box *box1 = obj1;
	struct Box *box2 = obj2;

	return FuncIntCompare(&box1->depth, &box2->depth);
}

void DumpList(struct ListNode *list_box)
{
	struct ListNode *head = list_box, *p = NULL;
	
	for (p = head->next; p != head; p = p->next)
	{
		struct Box *box = p->container;

		FuncBoxDump(box);
	}
	printf("\n");
}

void FilterList(struct ListNode *list_box, struct ListNode **tmp, int num_tmp)
{
	struct ListNode *head = list_box, *p = NULL, *n = NULL;
	struct Box *box = NULL;

	if (num_tmp <= 0)
		return;

	box = tmp[num_tmp - 1]->container; // backward

	while (box != NULL)
	{
		box->pick = 1;
		box = box->prev;
	}

	printf("\n");	
	for (p = head->next, n = p->next; p != head; p = n, n = p->next)
	{
		struct Box *box = p->container;

		if (box->pick)
		{
			FuncBoxDump(box);
			box->pick = 0; // reset
		}
		else
			ListNodeDelete(p);
	}
	printf("\n");
}

int LeftMostOrClosestIndex(struct ListNode* *tmp, int start, int end, struct Box *box, FuncObjCompare func_width_compare)
{
	int index = -1, index_closest = -1;

	while (start <= end)
	{
		int mid = (start + end) / 2;

		int compare = func_width_compare(tmp[mid]->container, box);

		if (compare == 0)
			index = mid;

		if (compare >= 0)
			end = mid - 1;
		else
		{
			index_closest = mid;
			start = mid + 1;
		}
	}

	if (index == -1)
		index = index_closest;

	return index;
}

void PickBox(struct ListNode *list_box, FuncObjCompare fun_obj_compare)
{
	int num_box = ListSize(list_box);

	struct ListNode **tmp = calloc(num_box, sizeof(struct ListNode*));
	int num_tmp = 0;

	struct ListNode *head = list_box, *p = NULL;

        for (p = head->next; p != head; p = p->next)
        {
                struct Box *box = p->container, *prev_box = NULL;

		int index = LeftMostOrClosestIndex(tmp, 0, num_tmp - 1, box, fun_obj_compare);
		// -1 ~ num_tmp - 1
		int update = 0;

//		FuncBoxDump(box);
//		printf("LeftMostOrClosestIndex %d\n", index);

		if (index == -1)
			update = 1;
		else if (fun_obj_compare(tmp[index]->container, box) < 0)
		{
			update = 1;
			prev_box = tmp[index]->container;
		}

		if (update)
		{
			tmp[index + 1] = p;

			if (index == num_tmp - 1)
				++num_tmp;

			box->prev = prev_box;
		}
	}

	FilterList(list_box, tmp, num_tmp);
}

void StackBox(int *heights, int *widths, int *depth, int num_box)
{
	struct ListNode list_box;
	
	int i = 0;

	ListInit(&list_box);

	for (i = 0; i < num_box; ++i)
	{
		struct Box *box = calloc(1, sizeof(struct Box));
		BoxInit(box, i, heights[i], widths[i], depth[i]);

		ListAddTail(&list_box, &box->hook);
	}

	ListMergeSort(&list_box, SortTypeNonDecreasing, FuncHeightCompare, FuncBoxDump);

	DumpList(&list_box);

	PickBox(&list_box, FuncWidthCompare);

	PickBox(&list_box, FuncDepthCompare);
}

int main(int argc, char *argv[])
{
/*
#define kNumBox 6
	int heights[kNumBox] = {65, 70, 56, 75, 60, 68};
	int widths[kNumBox] = {100, 150, 90, 190, 95, 110};
	int depths[kNumBox] = {100, 200, 150, 300, 125, 95};

	StackBox(heights, widths, depths, kNumBox);
*/

	int *heights = NULL;
	int *widths = NULL;
	int *depths = NULL;

	int i = 0, num_box = atoi(argv[1]);

	srand(time(NULL));

	heights = calloc(num_box, sizeof(int));
	widths = calloc(num_box, sizeof(int));
	depths = calloc(num_box, sizeof(int));

	for (i = 0; i < num_box; ++i)
	{
		heights[i] = rand() % 100;
		widths[i] = rand() % 100;
		depths[i] = rand() % 100;
	}

	StackBox(heights, widths, depths, num_box);

	return 0;
}

