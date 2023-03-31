
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <limits.h>
#include <time.h>

#include "../lib/helpers.h"

struct Hanoi
{
	int **piles;
	int num_pile;
	int *num_items;
};

void HanoiDump(struct Hanoi *hanoi)
{
	int i = 0, j = 0;

	for (i = 0; i < hanoi->num_pile; ++i)
	{
		for (j = 0; j < hanoi->num_items[i]; ++j)
			printf(" %d", hanoi->piles[i][j]);
		printf("\n");
	}

	printf("=====\n");
}

void HanoiInit(struct Hanoi *hanoi, int **piles, int num_pile, int *num_items)
{
	hanoi->piles = piles;
	hanoi->num_pile = num_pile;
	hanoi->num_items = num_items;
}

void HanoiMoveHelper(struct Hanoi *hanoi, int from, int num_item, int to)
{
	int mid = 3 - from - to;

	if (num_item == 0)
		return;

	HanoiMoveHelper(hanoi, from, num_item - 1, mid);

	hanoi->piles[to][hanoi->num_items[to]] = hanoi->piles[from][hanoi->num_items[from] - 1];
	++hanoi->num_items[to];
	--hanoi->num_items[from];

	HanoiDump(hanoi);

	HanoiMoveHelper(hanoi, mid, num_item - 1, to);
}

void HanoiMove(int **piles, int num_pile, int *num_items, int from, int to)
{
	struct Hanoi hanoi;

	HanoiInit(&hanoi, piles, num_pile, num_items);

	HanoiDump(&hanoi);

	HanoiMoveHelper(&hanoi, from, num_items[from], to);
}

int main(int argc, char *argv[])
{
#define kNumRow 3
	int num_col = atoi(argv[1]);
	int *num_items = calloc(kNumRow, sizeof(int));

	int **matrix = calloc(kNumRow, sizeof(int*));
	int i = 0, j = 0;

	for (i = 0; i < kNumRow; ++i)
	{
		matrix[i] = calloc(num_col, sizeof(int));
		num_items[i] = 0;
	}

	for (j = 0; j < num_col; ++j)
		matrix[0][j] = num_col - j;
	num_items[0] = num_col;
	
	HanoiMove(matrix, kNumRow, num_items, 0, 2);

	return 0;
}

