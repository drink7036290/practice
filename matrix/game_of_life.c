
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "lib/helpers.h"

struct Cell
{
	int val;
	int cn_live_neighbors;
};

void CellInit(struct Cell *cell, int val)
{
	cell->val = val;
	cell->cn_live_neighbors = 0;
}

void AddLiveToNeighbors(struct Cell **cloned, int num_row, int num_col, int x, int y)
{
	int i = 0;

	for (i = x - 1; i <= x + 1; ++i)
	{
		int j = 0;

		if ((i < 0) || (i >= num_row)) // outbound
			continue;

		for (j = y - 1; j <= y + 1; ++j)
		{
			if ((j < 0) || (j >= num_col)) // outbound
				continue;

			if ((i == x) && (j == y)) // skip self
				continue;

			++cloned[i][j].cn_live_neighbors;
		}
	}
}

void AddLivesToNeighbors(struct Cell **cloned, int num_row, int num_col)
{
	int i = 0;

	// saved to a list earlier and traverse if lives are few
	for (i = 0; i < num_row; ++i)
	{
		int j = 0;

		for (j = 0; j < num_col; ++j)
		{
			if (! cloned[i][j].val)
				continue;

			AddLiveToNeighbors(cloned, num_row, num_col, i, j);
		}
	}
}

void AssignMatrix(struct Cell **cloned, int **matrix, int num_row, int num_col)
{
	int i = 0;

	for (i = 0; i < num_row; ++i)
	{
		int j = 0;

		for (j = 0; j < num_col; ++j)
			matrix[i][j] = cloned[i][j].val;
	}
}

void UpdateMatrix(struct Cell **cloned, int num_row, int num_col)
{
	int i = 0;

	for (i = 0; i < num_row; ++i)
	{
		int j = 0;

		for (j = 0; j < num_col; ++j)
		{
			int val = cloned[i][j].val;
			int cn = cloned[i][j].cn_live_neighbors;

			if ((cn < 2) && val) // <2 : 1->0
				cloned[i][j].val = 0;

			else if ((cn == 3) && (val == 0)) // == 3 : 0->1
				cloned[i][j].val = 1;

			else if ((cn > 3) && val) // > 3 : 1->0
				cloned[i][j].val = 0;
		}
	}
}

void DumpMatrix(struct Cell **cloned, int num_row, int num_col)
{
	int i = 0;

	printf("===\n");

	for (i = 0; i < num_row; ++i)
	{
		int j = 0;

                for (j = 0; j < num_col; ++j)
			printf(" %d(%d)", cloned[i][j].val, cloned[i][j].cn_live_neighbors);
		printf("\n");
	}

	printf("===\n");
}

void FreeMatrix(struct Cell **cloned, int num_row, int num_col)
{
	int i = 0;

	for (i = 0; i < num_row; ++i)
		free(cloned[i]);

	free(cloned);
}

struct Cell** CloneMatrix(int **matrix, int num_row, int num_col)
{
	struct Cell **cloned = calloc(num_row, sizeof(struct Cell*));
	int i = 0;

	for (i = 0; i < num_row; ++i)
	{
		int j = 0;

		cloned[i] = calloc(num_col, sizeof(struct Cell));

		for (j = 0; j < num_col; ++j)
			CellInit(&cloned[i][j], matrix[i][j]);
	}

	return cloned;
}

int OriginalVal(int val)
{
	return ((val == 1) || (val == -1));
}

int NewVal(int val)
{
	return ((val == 1) || (val == 2));
}

int CountLiveNeighbors(int **matrix, int num_row, int num_col, int x, int y)
{
	int i = 0, count = 0;

	for (i = x - 1; i <= x + 1; ++i)
	{
		int j = 0;

		if ((i < 0) || (i >= num_row)) // outbound
			continue;

		for (j = y - 1; j <= y + 1; ++j)
		{
			if ((j < 0) || (j >= num_col)) // outbound
				continue;

			if ((i == x) && (j == y)) // skip self
				continue;

			if (OriginalVal(matrix[i][j]) == 1)
				++count;
		}
	}

	return count;
}

void UpdateMatrix2(int **matrix, int num_row, int num_col)
{
	int i = 0;

	for (i = 0; i < num_row; ++i)
	{
		int j = 0;

		for (j = 0; j < num_col; ++j)
		{
			int cn = CountLiveNeighbors(matrix, num_row, num_col, i, j);
			int val = OriginalVal(matrix[i][j]);

			if ((cn < 2) && val) // <2 : 1->-1
				matrix[i][j] = -1;

			else if ((cn == 3) && (val == 0)) // == 3 : 0->2
				matrix[i][j] = 2;

			else if ((cn > 3) && val) // > 3 : 1->-1
				matrix[i][j] = -1;
		}
	}
}

void RecoverMatrix(int **matrix, int num_row, int num_col)
{
	int i = 0;

	for (i = 0; i < num_row; ++i)
	{
		int j = 0;

		for (j = 0; j < num_col; ++j)
			matrix[i][j] = NewVal(matrix[i][j]);
	}
}

void GameOfLife(int **matrix, int num_row, int num_col)
{
/*	struct Cell **cloned = CloneMatrix(matrix, num_row, num_col);

	AddLivesToNeighbors(cloned, num_row, num_col);

	UpdateMatrix(cloned, num_row, num_col);

//	DumpMatrix(cloned, num_row, num_col);

	AssignMatrix(cloned, matrix, num_row, num_col);

	FreeMatrix(cloned, num_row, num_col);
*/
	UpdateMatrix2(matrix, num_row, num_col);

	RecoverMatrix(matrix, num_row, num_col);
}
/*
const int kNumRow = 4;
const int kNumCol = 3;
*/
const int kNumRow = 2;
const int kNumCol = 2;

int main(int argc, char *argv[])
{
	int **matrix = calloc(kNumRow, sizeof(int*));
        int i = 0;

        for (i = 0; i < kNumRow; ++i)
                matrix[i] = calloc(kNumCol, sizeof(int));
/*
	matrix[0][0] = 0;
	matrix[0][1] = 1;
	matrix[0][2] = 0;

	matrix[1][0] = 0;
	matrix[1][1] = 0;
	matrix[1][2] = 1;

	matrix[2][0] = 1;
	matrix[2][1] = 1;
	matrix[2][2] = 1;

	matrix[3][0] = 0;
	matrix[3][1] = 0;
	matrix[3][2] = 0;
*/
	matrix[0][0] = 1;
        matrix[0][1] = 1;
	
	matrix[1][0] = 1;
        matrix[1][1] = 0;


	for (i = 0; i < kNumRow; ++i)
        {
                int j = 0;

                for (j = 0; j < kNumCol; ++j)
			printf(" %d", matrix[i][j]);
                printf("\n");
	}

	GameOfLife(matrix, kNumRow, kNumCol);

	printf("==========\n");

	for (i = 0; i < kNumRow; ++i)
        {
                int j = 0;

                for (j = 0; j < kNumCol; ++j)
			printf(" %d", matrix[i][j]);
                printf("\n");
	}

	return 0;
}

