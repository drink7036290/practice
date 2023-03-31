
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../lib/helpers.h"

struct Submatrix
{
	// top left
	int r0;
	int c0;

	// bottom right
	int r1;
	int c1;

	int sum;
};

void SubmatrixSet(struct Submatrix *submatrix, int r0, int c0, int r1, int c1, int sum)
{
	submatrix->r0 = r0;
	submatrix->c0 = c0;

	submatrix->r1 = r1;
	submatrix->c1 = c1;

	submatrix->sum = sum;
}

void SubmatrixDump(struct Submatrix *submatrix)
{
	printf("top left (%d,%d) right bottom (%d,%d) max %d\n"
		, submatrix->r0, submatrix->c0
		, submatrix->r1, submatrix->c1
		, submatrix->sum);
}

void SubmatrixInit(struct Submatrix *submatrix)
{
	SubmatrixSet(submatrix, -1, -1, -1, -1, 0);
}

struct Params
{
	int **nums;

	int *col_sum;
	int num_col;

	struct Submatrix *submatrix;
};

void ParamsInit(struct Params *params, int **nums, int num_col, struct Submatrix *submatrix)
{
	params->nums = nums;

	params->col_sum = calloc(num_col, sizeof(int));
	params->num_col = num_col;

	params->submatrix = submatrix;
}

void UpdateSubmatrix(struct Params *params, int row_start, int row_end)
{
	int col = 0, col_start = 0, col_end = 0;
	int accu = 0, max = INT_MIN;

	for (col = 0; col <= params->num_col; ++col)	
	{
		params->col_sum[col] += params->nums[row_end][col];
		printf(" (%d) %d", col, params->col_sum[col]);
	}
	printf("\n");

	if (params->num_col > 0)
	{
		accu = params->col_sum[0];
		max = accu;
	}

	for (col = 1; col <= params->num_col; ++col)
	{
		int sum = accu + params->col_sum[col];

		if (sum < params->col_sum[col])
		{
			accu = params->col_sum[col];
			col_start = col;
		}
		else
			accu = sum;

		if (accu > max)
		{
			max = accu;
			col_end = col;
		}
	}

	if (params->submatrix->sum < max)
		SubmatrixSet(params->submatrix, row_start, col_start, row_end, col_end, max);
}

struct Submatrix* GetMaxSubmatrix(int **nums, int num_row, int num_col)
{
	int row_start = 0;

	struct Submatrix *submatrix = calloc(1, sizeof(struct Submatrix));
	SubmatrixInit(submatrix);

	for (row_start = 0; row_start < num_row; ++row_start)
	{
		int row_end = 0;

		struct Params params;
		ParamsInit(&params, nums, num_col, submatrix);

		for (row_end = row_start; row_end < num_row; ++row_end)
			UpdateSubmatrix(&params, row_start, row_end);
	}

	return submatrix;
}

void Test(void)
{
	struct Submatrix *submatrix = NULL;

#define kNumRow 3
#define kNumCol 5

	int matrix_ref[kNumRow][kNumCol] = {
		  { 9, -8,  1,  3, -2}
		, {-3,  7,  6, -2,  4}
		, { 6, -4, -4,  8, -7}
	};
	
	int **matrix = calloc(kNumRow, sizeof(int*));
	int i = 0, j = 0;

	for (i = 0; i < kNumRow; ++i)
	{
		matrix[i] = calloc(kNumCol, sizeof(int));
	
		for (j = 0; j < kNumCol; ++j)
			matrix[i][j] = matrix_ref[i][j];
	}

	submatrix = GetMaxSubmatrix(matrix, kNumRow, kNumCol);

	SubmatrixDump(submatrix);
}

int main(int argc, char *argv[])
{
	Test();

	return 0;
}

