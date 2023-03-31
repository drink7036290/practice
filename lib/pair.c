
#include <stdlib.h>
#include <limits.h>

#include "pair.h"


int PairOverlap(struct Pair *pair1, struct Pair *pair2)
{
	if (pair1->x == pair2->x)
		return 1;

	if (pair1->x == pair2->y)
		return 1;

	if (pair1->y == pair2->x)
		return 1;

	if (pair1->y == pair2->y)
		return 1;
	
	return 0;
}

int PairEqual(struct Pair *pair1, struct Pair *pair2)
{
	if (pair1->x != pair2->x)
		return 0;

	if (pair1->y != pair2->y)
		return 0;

	return 1;
}

void PairDump(struct Pair *pair)
{
	printf("[Pair] %d %d\n", pair->x, pair->y);
}

void PairExit(struct Pair *pair)
{
	// do nothing
}

void PairInit(struct Pair *pair, int x, int y)
{
	pair->x = x;
	pair->y = y;
}

// ==============================================================

void PairsDump(struct Pairs *pairs)
{
	int i = 0;

	for (i = 0; i < pairs->num_pair; ++i)
	{
		struct Pair *pair = &pairs->array[i];
		printf(" [%d, %d]", pair->x, pair->y);
	}

	printf("\n");
}

void PairsInit(struct Pairs *pairs, int num_pair)
{
	int i = 0;

	pairs->num_pair = num_pair;

	pairs->array = malloc(num_pair * sizeof(struct Pair));

	for (i = 0; i < num_pair; ++i)
		PairInit(&pairs->array[i], INT_MIN, INT_MAX);
}
