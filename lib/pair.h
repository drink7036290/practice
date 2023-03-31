
#ifndef PAIR_H_
#define PAIR_H_

#include "stdio.h"

struct Pair
{
	int x;
	int y;
};

int PairOverlap(struct Pair *pair1, struct Pair *pair2);

int PairEqual(struct Pair *pair1, struct Pair *pair2);

void PairDump(struct Pair *pair);

void PairExit(struct Pair *pair);

void PairInit(struct Pair *pair, int x, int y);

struct Pairs
{
	struct Pair *array;
	int num_pair;
};

void PairsDump(struct Pairs *pairs);
void PairsInit(struct Pairs *pairs, int num_pair);

#endif
