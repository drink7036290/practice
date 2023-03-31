
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../lib/helpers.h"

struct PilesInfo
{
	int **piles;
	int num_pile;
	int *piles_col_size;
};

void PilesInit(struct PilesInfo *piles_info, int **piles, int num_pile, int *piles_col_size)
{
	piles_info->piles = piles;
	piles_info->num_pile = num_pile;
	piles_info->piles_col_size = piles_col_size;
}

int MaxVal(struct PilesInfo *piles_info, int k, int index, int **map)
{
	int i = 0, cn = 0;
	int self_val = 0, max_val = 0;

	int *pile = NULL;
	int pile_size = 0;

	if (index >= piles_info->num_pile)
		return 0;

	if (map[k][index] > 0)
		return map[k][index];

	pile = piles_info->piles[index];		
	pile_size = piles_info->piles_col_size[index];

	// without
	max_val = Max(max_val, MaxVal(piles_info, k, index + 1, map));

	cn = Min(k, pile_size);

	// with
	for (i = 0; i < cn; ++i)
	{
		int sub_val = MaxVal(piles_info, k - (i + 1), index + 1, map);

		self_val += pile[i];

		max_val = Max(max_val, self_val + sub_val);
	}

	map[k][index]  = max_val;
	return max_val;
}

int MaxValWrapper(struct PilesInfo *piles_info, int k)
{
	int **map = calloc(k + 1, sizeof(int *));
	int i = 0, val = 0;

	for (i = 0; i < k + 1; ++i)
		map[i] = calloc(piles_info->num_pile, sizeof(int));

	val = MaxVal(piles_info, k, 0, map);

	for (i = 0; i < k + 1; ++i)
		free(map[i]);
	free(map);

	return val;
}

int GetMaxVal(int **piles, int num_pile, int *piles_col_size, int k)
{
	struct PilesInfo piles_info;

	PilesInit(&piles_info, piles, num_pile, piles_col_size);

	return MaxValWrapper(&piles_info, k);
}

//#define kNumPile 2
#define kNumPile 7

int main(int argc, char *argv[])
{
//	int piles_col_size[kNumPile] = {3, 3};
	int piles_col_size[kNumPile] = {1, 1, 1, 1, 1, 1, 7};

	int **piles = calloc(kNumPile, sizeof(int*));
        int i = 0;

        for (i = 0; i < kNumPile; ++i)
                piles[i] = calloc(piles_col_size[i], sizeof(int));
/*
	piles[0][0] = 1;
	piles[0][1] = 100;
	piles[0][2] = 3;

	piles[1][0] = 7;
	piles[1][1] = 8;
	piles[1][2] = 9;
*/
	piles[0][0] = 100;
	piles[1][0] = 100;
	piles[2][0] = 100;
	piles[3][0] = 100;
	piles[4][0] = 100;
	piles[5][0] = 100;

	piles[6][0] = 1;
	piles[6][1] = 1;
	piles[6][2] = 1;
	piles[6][3] = 1;
	piles[6][4] = 1;
	piles[6][5] = 1;
	piles[6][6] = 700;

	printf("%d\n", GetMaxVal(piles, kNumPile, piles_col_size, atoi(argv[1])));

	return 0;
}

