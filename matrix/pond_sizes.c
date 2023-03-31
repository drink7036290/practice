
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

struct MatrixInfo
{
	int **matrix;
	int num_row;
	int num_col;

//	int **visited;
};

void MatrixInfoInit(struct MatrixInfo *matrix_info, int **matrix, int num_row, int num_col)
{
//	int i = 0;

	matrix_info->matrix = matrix;
	matrix_info->num_row = num_row;
	matrix_info->num_col = num_col;
/*
	matrix_info->visited = calloc(num_row, sizeof(int*));
	for (i = 0; i < num_row; ++i)
		matrix_info->visited[i] = calloc(num_col, sizeof(int));
*/
}

int DFS(struct MatrixInfo *matrix_info, int row, int col)
{
	int i = 0, j = 0, sum = 1; // self

	if ((row < 0) || (row >= matrix_info->num_row))
		return 0;
	if ((col < 0) || (col >= matrix_info->num_col))
		return 0;

	if (matrix_info->matrix[row][col] != 0)
		return 0;
/*
	if (matrix_info->visited[row][col])
		return 0;
	matrix_info->visited[row][col] = 1;
*/
	matrix_info->matrix[row][col] = -1;

	for (i = -1; i <= 1; ++i)
	{
		for (j = -1; j <= 1; ++j)
			sum += DFS(matrix_info, row + i, col + j);
	}

	return sum;
}

void PrintPondSizes(int **matrix, int num_row, int num_col)
{
	int i = 0, j = 0, sum = 0;

	struct MatrixInfo matrix_info;

	MatrixInfoInit(&matrix_info, matrix, num_row, num_col);

	for (i = 0; i < num_row; ++i)
	{
		for (j = 0; j < num_col; ++j)
		{
			if ((sum = DFS(&matrix_info, i, j)) > 0)
				printf(" %d", sum);
		}
	}
	printf("\n");
}


int main(int argc, char *argv[])
{
#define kNumRow 4
#define kNumCol 4

	int matrix_ref[kNumRow][kNumCol] = {
		  {0, 2, 1, 0}
		, {0, 1, 0, 1}
		, {1, 1, 0, 1}
		, {0, 1, 0, 1}
	};
	
	int **matrix = calloc(kNumRow, sizeof(int*));
	int i = 0, j = 0;

	for (i = 0; i < kNumRow; ++i)
	{
		matrix[i] = calloc(kNumCol, sizeof(int));
	
		for (j = 0; j < kNumCol; ++j)
			matrix[i][j] = matrix_ref[i][j];
	}

	PrintPondSizes(matrix, kNumRow, kNumCol);

	return 0;
}

