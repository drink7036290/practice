
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

struct Pair
{
	int r;
	int c;
};

void PairInit(struct Pair *pair, int r, int c)
{
	pair->r = r;
	pair->c = c;
}

struct Params
{
	int **matrix;
	int num_row;
	int num_col;
	int k;

	int *col_sum;

	struct Pair top_left;
	struct Pair bottom_right;
	int sum;
};

void ParamsInit(struct Params *params, int **matrix, int num_row, int num_col, int k)
{
	params->matrix = matrix;
	params->num_row = num_row;
	params->num_col = num_col;
	params->k = k;

	params->col_sum = calloc(num_col, sizeof(int));

	PairInit(&params->top_left, -1, -1);
	PairInit(&params->bottom_right, -1, -1);
	params->sum = INT_MIN;
}

void FindMaxNoLargerHelper(struct Params *params, int row_start, int row_end)
{
	int col = 0;
	int i = 0, j = 0, sum = 0;

	printf("row start %d end %d\n", row_start, row_end);

	printf("col_sum");
	for (col = 0; col < params->num_col; ++col)
	{
		params->col_sum[col] += params->matrix[row_end][col];
		printf(" %d", params->col_sum[col]);
	}
	printf("\n");

	while (j < params->num_col)
	{
		printf("i %d j %d\n", i, j);

		sum += params->col_sum[j++];
		while ((sum > params->k) && (i < j))
			sum -= params->col_sum[i++];

		printf("k %d sum %d curr %d\n", params->k, sum, params->sum);

		if (sum <= params->sum)
			continue;
		if (i >= j) // [ i, j )
			continue;

		printf("update sum %d to %d\n", params->sum, sum);

		params->sum = sum;
		PairInit(&params->top_left, row_start, i);
		PairInit(&params->bottom_right, row_end, j - 1);
	}
}

int FindMaxNoLarger(int **matrix, int num_row, int num_col, int k)
{
	struct Params params;
	int row_start = 0;

	ParamsInit(&params, matrix, num_row, num_col, k);

	for (row_start = 0; row_start < num_row; ++row_start)
	{
		int row_end = 0, col = 0;

		// reset
		for (col = 0; col < num_col; ++col)
			params.col_sum[col] = 0;

		for (row_end = row_start; row_end < num_row; ++row_end)
			FindMaxNoLargerHelper(&params, row_start, row_end);
	}

	printf("(%d,%d) -> (%d, %d) sum %d\n"
		, params.top_left.r
		, params.top_left.c
		, params.bottom_right.r
		, params.bottom_right.c
		, params.sum);

	return params.sum;
}

void Test(void)
{
#define kNumRow 1
#define kNumCol 3

	int matrix_ref[kNumRow][kNumCol] = {
//		  { 1,  0,  1}
//		, { 0, -2,  3}
		  { 2,  2, -1}
	};
	int k = 3;

	int **matrix = calloc(kNumRow, sizeof(int*));
	int i = 0, j = 0;

	for (i = 0; i < kNumRow; ++i)
	{
		matrix[i] = calloc(kNumCol, sizeof(int));
	
		for (j = 0; j < kNumCol; ++j)
			matrix[i][j] = matrix_ref[i][j];
	}

	FindMaxNoLarger(matrix, kNumRow, kNumCol, k);
}

int main(int argc, char *argv[])
{
	Test();

	return 0;
}

