
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

	int **BIT; // binary indexed tree
};

int BITShift(int index)
{
	return (index & (-index));
}

int MatrixInfoSumUntil(struct MatrixInfo *matrix_info, int row, int col)
{
	int i = 0, sum = 0;

	for (i = row + 1; i > 0; i -= BITShift(i))
	{
		int j = 0;

		for (j = col + 1; j > 0; j -= BITShift(j))
			sum += matrix_info->BIT[i][j];
	}

	return sum;
}

int MatrixInfoRangeSum(struct MatrixInfo *matrix_info, int r0, int c0, int r1, int c1)
{
	int left = MatrixInfoSumUntil(matrix_info, r1, c0 - 1);
	int up = MatrixInfoSumUntil(matrix_info, r0 - 1, c1);
	int left_up = MatrixInfoSumUntil(matrix_info, r0 - 1, c0 - 1);

	return MatrixInfoSumUntil(matrix_info, r1, c1) - left - up + left_up;
}

void MatrixInfoUpdate(struct MatrixInfo *matrix_info, int row, int col, int val)
{
	int i = 0;

	int diff = val - matrix_info->matrix[row][col];
	matrix_info->matrix[row][col] = val;

	for (i = row + 1; i <= matrix_info->num_row; i += BITShift(i))
	{
		int j = 0;

		for (j = col + 1; j <= matrix_info->num_col; j += BITShift(j))
			matrix_info->BIT[i][j] += diff;
	}
}

void MatrixInfoDump(struct MatrixInfo *matrix_info)
{
	int i = 0;

        for (i = 0; i <= matrix_info->num_row; ++i) // <=
	{
		int j = 0;

		for (j = 0; j <= matrix_info->num_col; ++j) // <=
			printf(" %d", matrix_info->BIT[i][j]);
		printf("\n");
	}
	printf("\n");
}

void MatrixInfoExit(struct MatrixInfo *matrix_info)
{
	int i = 0;

	for (i = 0; i <= matrix_info->num_row; ++i) // <=
		free(matrix_info->BIT[i]);
	free(matrix_info->BIT);
}

void MatrixInfoInit(struct MatrixInfo *matrix_info, int **matrix, int num_row, int num_col)
{
	int i = 0, j = 0;

	matrix_info->matrix = matrix;
	matrix_info->num_row = num_row;
	matrix_info->num_col = num_col;

	matrix_info->BIT = calloc(num_row + 1, sizeof(int*)); // + 1

	for (i = 0; i <= num_row; ++i) // <=
		matrix_info->BIT[i] = calloc(num_col + 1, sizeof(int)); // + 1

	for (i = 0; i < num_row; ++i)
	{
		for (j = 0; j < num_col; ++j)
			matrix_info->BIT[i + 1][j + 1] = matrix_info->BIT[i + 1][j] + matrix_info->BIT[i][j + 1] - matrix_info->BIT[i][j] + matrix[i][j];

		MatrixInfoDump(matrix_info);
	}
	
	for (i = num_row; i > 0; --i) // >
	{
		for (j = num_col; j > 0; --j) // >
		{
			int left = matrix_info->BIT[i][j - BITShift(j)];
			int up = matrix_info->BIT[i - BITShift(i)][j];
			int left_up = matrix_info->BIT[i - BITShift(i)][j - BITShift(j)];

			matrix_info->BIT[i][j] -= (left + up - left_up);
		}

		MatrixInfoDump(matrix_info);
	}
}

void Test(void)
{
#define kNumRow 3
#define kNumCol 4

	int matrix_ref[kNumRow][kNumCol] = {
		  {  0, -3, -8,  3}
		, { -9,  3,  5,  3}
		, {  2,  3, -5,  3}
	};
	
	int **matrix = calloc(kNumRow, sizeof(int*));
	int i = 0, j = 0;

	struct MatrixInfo matrix_info;

	for (i = 0; i < kNumRow; ++i)
	{
		matrix[i] = calloc(kNumCol, sizeof(int));
	
		for (j = 0; j < kNumCol; ++j)
			matrix[i][j] = matrix_ref[i][j];
	}

	MatrixInfoInit(&matrix_info, matrix, kNumRow, kNumCol);

	printf("%d\n", MatrixInfoRangeSum(&matrix_info, 0, 1, 1, 2));

	MatrixInfoUpdate(&matrix_info, 0, 1, 1);

	printf("%d\n", MatrixInfoRangeSum(&matrix_info, 0, 1, 1, 2));

	MatrixInfoExit(&matrix_info);
}

int main(int argc, char *argv[])
{
	Test();

	return 0;
}

