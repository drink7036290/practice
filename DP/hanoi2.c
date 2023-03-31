
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

struct Params
{
	int **piles;
	int num_pile;
	int num_disk;
	int *last_index;
};

void ParamsInit(struct Params *params, int **piles, int num_pile, int num_disk)
{
	int i = 0;

	params->piles = piles;
	params->num_pile = num_pile;
	params->num_disk = num_disk;

	params->last_index = calloc(num_pile, sizeof(int));

	for (i = 0; i < num_pile; ++i)
		params->last_index[i] = -1;
}

void ParamsDump(struct Params *params)
{
	int i = 0, j = 0;

	for (i = 0; i < params->num_pile; ++i)
	{
		for (j = 0; j <= params->last_index[i]; ++j)
			printf(" %d", params->piles[i][j]);
		printf("\n");
	}

	printf("=====\n");
}

void HanoiHelper(struct Params *params, int cn, int pile_from, int pile_to)
{
	int pile_mid = params->num_pile - pile_from - pile_to;
	int val = 0;

	if (cn <= 0)
		return;

	HanoiHelper(params, cn - 1, pile_from, pile_mid);

	val = params->piles[pile_from][params->last_index[pile_from]--];
	params->piles[pile_to][++params->last_index[pile_to]] = val;

	ParamsDump(params);

	HanoiHelper(params, cn - 1, pile_mid, pile_to);
}

void Hanoi(int **piles, int num_pile, int num_disk, int pile_from, int pile_to)
{
	struct Params params;
	ParamsInit(&params, piles, num_pile, num_disk);

	params.last_index[pile_from] = num_disk - 1;

	ParamsDump(&params);

	HanoiHelper(&params, num_disk, pile_from, pile_to);
}

int main(int argc, char *argv[])
{
#define kNumRow 3
	int num_col = atoi(argv[1]);
	
	int **matrix = calloc(kNumRow, sizeof(int*));
	int i = 0, j = 0;

	for (i = 0; i < kNumRow; ++i)
		matrix[i] = calloc(num_col, sizeof(int));

	for (j = 0; j < num_col; ++j)
		matrix[0][j] = num_col - j;

	Hanoi(matrix, kNumRow, num_col, 0, 2);

	return 0;
}

