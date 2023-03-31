
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../lib/helpers.h"

struct MatrixInfo
{
	int **matrix;
	int num_row;
	int num_col;

	int org_color;
	int new_color;
};

void MatrixInfoDump(struct MatrixInfo *matrix_info)
{
	int i = 0;

	for (i = 0; i < matrix_info->num_row; ++i)
	{
		int j = 0;

		for (j = 0; j < matrix_info->num_col; ++j)
			printf(" %d", matrix_info->matrix[i][j]);
		printf("\n");
	}
}

void MatrixInfoInit(struct MatrixInfo *matrix_info, int **matrix, int num_row, int num_col, int org_color, int new_color)
{
	matrix_info->matrix = matrix;
	matrix_info->num_row = num_row;
	matrix_info->num_col = num_col;

	matrix_info->org_color = org_color;
	matrix_info->new_color = new_color;
}

void PaintFillHelper(struct MatrixInfo *matrix_info, int r, int c)
{
	if ((r < 0) || (r >= matrix_info->num_row))
		return;

	if ((c < 0) || (c >= matrix_info->num_col))
		return;

	if (matrix_info->matrix[r][c] != matrix_info->org_color)
		return;

	// change self
	matrix_info->matrix[r][c] = matrix_info->new_color;

	PaintFillHelper(matrix_info, r - 1, c);
	PaintFillHelper(matrix_info, r, c - 1);
	PaintFillHelper(matrix_info, r + 1, c);
	PaintFillHelper(matrix_info, r, c + 1);
}

void PaintFill(int **matrix, int num_row, int num_col, int r, int c, int color)
{
	struct MatrixInfo matrix_info;

	if ((r < 0) || (r >= num_row))
		return;

	if ((c < 0) || (c >= num_col))
		return;

	if (matrix[r][c] == color)
		return;

	MatrixInfoInit(&matrix_info, matrix, num_row, num_col, matrix[r][c], color);

	PaintFillHelper(&matrix_info, r, c);

	MatrixInfoDump(&matrix_info);
}

int main(int argc, char *argv[])
{
#define kNumRow 3
#define kNumCol 3
	int matrix_ref[kNumRow][kNumCol] = {
		  {1, 1, 1}
		, {2, 1, 2}
		, {3, 3, 3}
	};

	int r = 1, c = 0, color = 4;

	int **matrix = calloc(kNumRow, sizeof(int*));
	int i = 0;

	for (i = 0; i < kNumRow; ++i)
	{
		int j = 0;

		matrix[i] = calloc(kNumCol, sizeof(int));

		for (j = 0; j < kNumCol; ++j)
			matrix[i][j] = matrix_ref[i][j];
	}

	PaintFill(matrix, kNumRow, kNumCol, r, c, color);

	return 0;
}

