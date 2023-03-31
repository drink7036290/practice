
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../lib/helpers.h"

#include "../lib/list_node.h"

struct Person
{
	int index;
	int height;
	int weight;

	int pick;
	struct Person *prev;

	struct ListNode hook;
};

void PersonInit(struct Person *person, int index, int height, int weight)
{
	person->index = index;
	person->height = height;
	person->weight = weight;

	person->pick = 0;
	person->prev = NULL;

	ListNodeInit(&person->hook, person);
}

void FuncPersonDump(void *obj)
{
	struct Person *person = obj;

	printf("index %d height %d weight %d\n", person->index, person->height, person->weight);
}

int FuncWeightCompare(void *obj1, void *obj2)
{
	struct Person *person1 = obj1;
	struct Person *person2 = obj2;

	return FuncIntCompare(&person1->weight, &person2->weight);
}

int FuncHeightCompare(void *obj1, void *obj2)
{
	struct Person *person1 = obj1;
	struct Person *person2 = obj2;

	return FuncIntCompare(&person1->height, &person2->height);
}

void DumpList(struct ListNode *list_person)
{
	struct ListNode *head = list_person, *p = NULL;
	
	for (p = head->next; p != head; p = p->next)
	{
		struct Person *person = p->container;

		FuncPersonDump(person);
	}
	printf("\n");
}

int* ConvertList(struct ListNode *list_person)
{
	int *results = calloc(ListSize(list_person), sizeof(int));
	int index = 0;

	struct ListNode *head = list_person, *p = NULL;

	printf("\n");	
	for (p = head->next; p != head; p = p->next)
	{
		struct Person *person = p->container;

		if (! person->pick)
			continue;

		FuncPersonDump(person);

		results[index++] = person->index;
	}
	printf("\n");

	return results;
}

int LeftMostOrClosestIndex(struct ListNode* *tmp, int start, int end, struct Person *person, FuncObjCompare func_weight_compare)
{
	int index = -1, index_closest = -1;

	while (start <= end)
	{
		int mid = (start + end) / 2;

		int compare = func_weight_compare(tmp[mid]->container, person);

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

void PickPeople(struct ListNode *list_person)
{
	int num_person = ListSize(list_person);
	struct Person *person = NULL;

	struct ListNode **tmp = calloc(num_person, sizeof(struct ListNode*));
	int num_tmp = 0;

	struct ListNode *head = list_person, *p = NULL;

        for (p = head->next; p != head; p = p->next)
        {
                struct Person *prev_person = NULL;

		person = p->container;

		int index = LeftMostOrClosestIndex(tmp, 0, num_tmp - 1, person, FuncWeightCompare);
		// -1 ~ num_tmp - 1
		int update = 0;

//		FuncPersonDump(person);
//		printf("LeftMostOrClosestIndex %d\n", index);

		if (index == -1)
			update = 1;
		else if (FuncWeightCompare(tmp[index]->container, person) < 0)
		{
			update = 1;
			prev_person = tmp[index]->container;
		}

		if (update)
		{
			tmp[index + 1] = p;

			if (index == num_tmp - 1)
				++num_tmp;

			person->prev = prev_person;
		}
	}

	if (num_tmp <= 0)
		return;

	person = tmp[num_tmp - 1]->container; // backward

	while (person != NULL)
	{
		person->pick = 1;
		person = person->prev;
	}
}

int* CircusTower(int *heights, int *weights, int num_person)
{
	struct ListNode list_person;
	
	int i = 0;

	ListInit(&list_person);

	for (i = 0; i < num_person; ++i)
	{
		struct Person *person = calloc(1, sizeof(struct Person));
		PersonInit(person, i, heights[i], weights[i]);

		ListAddTail(&list_person, &person->hook);
	}

	ListMergeSort(&list_person, SortTypeNonDecreasing, FuncHeightCompare, FuncPersonDump);

	DumpList(&list_person);

	PickPeople(&list_person);

	return ConvertList(&list_person);
}

int main(int argc, char *argv[])
{
#define kNumPerson 6
	int heights[kNumPerson] = {65, 70, 56, 75, 60, 68};
	int weights[kNumPerson] = {100, 150, 90, 190, 95, 110};

	CircusTower(heights, weights, kNumPerson);
/*
	int *heights = NULL;
	int *weights = NULL;

	int i = 0, num_person = atoi(argv[1]);

	srand(time(NULL));

	heights = calloc(num_person, sizeof(int));
	weights = calloc(num_person, sizeof(int));

	for (i = 0; i < num_person; ++i)
	{
		heights[i] = 100 + rand() % (200 - 100 + 1);
		weights[i] = rand() % 100;
	}

	CircusTower(heights, weights, num_person);
*/
	return 0;
}

