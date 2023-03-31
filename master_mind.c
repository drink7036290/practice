
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include "lib/hash.h"

struct IndexInfo
{
	int index;

	int hit;
	int pseudo_hit;

	struct ListNode hook;
};

void IndexInfoInit(struct IndexInfo *index_info, int index)
{
	index_info->index = index;

	index_info->hit = 0;
	index_info->pseudo_hit = 0;

	ListNodeInit(&index_info->hook, index_info);
}

void AddIndexToList(struct ListNode *list, int index)
{
	struct IndexInfo *index_info = calloc(1, sizeof(struct IndexInfo));

	IndexInfoInit(index_info, index);

	ListAddTail(list, &index_info->hook);
}

void CalculateHit(struct ListNode *list, int index, int *hit, int *pseudo_hit)
{
	struct ListNode *head = list, *p = NULL;
	struct IndexInfo *index_info = NULL;

	for (p = head->next; p != head; p = p->next)
	{
		index_info = p->container;

		if (index_info->index == index)
		{
			if (index_info->pseudo_hit == 1)
				--*pseudo_hit;

			ListNodeDelete(p);

			++*hit;

			return;
		}
	}

	if (index_info == NULL)
		return;

	if (index_info->pseudo_hit == 0)
	{
		index_info->pseudo_hit = 1;
		++*pseudo_hit;
	}
}

void PrepareSolution(struct Hash *hash, char *solution)
{
	char *s = solution;
	int index = 0;

	while (*s != '\0')
	{
		struct ListNode *list = NULL;
		int key = *s - 'A';

		if ((list = HashGet(hash, &key)) == NULL)
		{
			int *new_key = calloc(1, sizeof(int));
			list = calloc(1, sizeof(struct ListNode));

			*new_key = key;
			ListInit(list);

			HashInsert(hash, new_key, list);
		}	

		AddIndexToList(list, index);

		++index;
		++s;
	}
}

void CheckGuess(struct Hash *hash, char *guess)
{
	char *g = guess;
	int index = 0;
	int hit = 0, pseudo_hit = 0;

	while (*g != '\0')
	{
		struct ListNode *list = NULL;
		int key = *g - 'A';

		if ((list = HashGet(hash, &key)) != NULL)
			CalculateHit(list, index, &hit, &pseudo_hit);

		++index;
		++g;
	}

	printf("hit %d pseudo_hit %d\n", hit, pseudo_hit);
}

void PrintResults(char *solution, char *guess)
{
	struct Hash hash;

	HashInit(&hash, FuncIntToInt, FuncIntCompare);

	PrepareSolution(&hash, solution);

	CheckGuess(&hash, guess);
}

int main(int argc, char *argv[])
{
	char solution[] = "RGBY";
	char guess[] = "GGRR";

	PrintResults(solution, guess);

	return 0;
}

