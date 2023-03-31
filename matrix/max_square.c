
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "../lib/helpers.h"
#include "../lib/pair.h"

struct MatrixInfo
{
	int **matrix;

	int num_row;
	int num_col;

	struct Pair **matrix_cn;

	int **matrix_cn2;
	int *cn3;
};

void MatrixInfoInit(struct MatrixInfo *matrix_info, int **matrix, int num_row, int num_col)
{
	int i = 0;

	matrix_info->matrix = matrix;
	
	matrix_info->num_row = num_row;
	matrix_info->num_col = num_col;

	matrix_info->matrix_cn = calloc(num_row, sizeof(struct Pair*));
	for (i = 0; i < num_row; ++i)
		matrix_info->matrix_cn[i] = calloc(num_col, sizeof(struct Pair));

	matrix_info->matrix_cn2 = calloc(num_row, sizeof(int*));
	for (i = 0; i < num_row; ++i)
		matrix_info->matrix_cn2[i] = calloc(num_col, sizeof(int));

	matrix_info->cn3 = calloc(num_col, sizeof(int));
}

struct Square
{
	int x; // top_left
	int y; // top_left

	int len;
};

void SquareDump(struct Square *square)
{
	printf("top left x %d y %d len %d\n", square->x, square->y, square->len);
}

void SquareAssign(struct Square *square, int x, int y, int len)
{
	square->x = x;
	square->y = y;

	square->len = len;
}

void SquareInit(struct Square *square)
{
	square->x = -1;
	square->y = -1;

	square->len = 0;
}

int IsSquare(struct MatrixInfo *matrix_info, int x, int y, int len)
{
/*
	int i = 0, j = 0;

	for (i = x; i < x + len; ++i)
	{
		if (! matrix_info->matrix[i][y])
			return 0;

		if (! matrix_info->matrix[i][y + len - 1])
			return 0;
	}

	for (j = y; j < y + len; ++j)
	{
		if (! matrix_info->matrix[x][j])
			return 0;

		if (! matrix_info->matrix[x + len - 1][j])
			return 0;
	}

	return 1;
*/

	struct Pair *bottom_right = &matrix_info->matrix_cn[x + len - 1][y + len - 1];

	struct Pair *bottom_left = &matrix_info->matrix_cn[x + len - 1][y];
	struct Pair *top_right = &matrix_info->matrix_cn[x][y + len - 1];

	if (bottom_left->x < len)
                return 0;

	if (top_right->y < len)
                return 0;

	if (bottom_right->x < len)
		return 0;

	if (bottom_right->y < len)
		return 0;

	return 1;
}

int FindMaxSquare(struct MatrixInfo *matrix_info, int len, struct Square* square)
{
	int cn_row = matrix_info->num_row - len + 1;
	int cn_col = matrix_info->num_col - len + 1;

	int i = 0, j = 0;

	for (i = 0; i < cn_row; ++i)
	{
		for (j = 0; j < cn_col; ++j)
		{
			if (! IsSquare(matrix_info, i, j, len))
				continue;
			
			SquareAssign(square, i, j, len);
			return 1;
		}
	}

	return 0;
}

void PrepareMatrixCount(struct MatrixInfo *matrix_info)
{
	int i = 0;

	printf("%s\n", __FUNCTION__);
	for (i = 0; i < matrix_info->num_row; ++i)
	{
		int j = 0;

		for (j = 0; j < matrix_info->num_col; ++j)
		{
			if (! matrix_info->matrix[i][j])
			{
				printf(" (%d, %d)", matrix_info->matrix_cn[i][j].x, matrix_info->matrix_cn[i][j].y);
				continue;
			}

			matrix_info->matrix_cn[i][j].x = 1;
			matrix_info->matrix_cn[i][j].y = 1;

			if (i - 1 >= 0)
				matrix_info->matrix_cn[i][j].x += matrix_info->matrix_cn[i - 1][j].x;

			if (j - 1 >= 0)
				matrix_info->matrix_cn[i][j].y += matrix_info->matrix_cn[i][j - 1].y;

			printf(" (%d, %d)", matrix_info->matrix_cn[i][j].x, matrix_info->matrix_cn[i][j].y);
		}
		printf("\n");
	}
}

void GetMaxSquare(struct MatrixInfo *matrix_info)
{
	int i = 0, j = 0;

	int bottom_right_x = -1, bottom_right_y = -1;
	int len = 0;

	printf("%s\n", __FUNCTION__);
	for (i = 0; i < matrix_info->num_row; ++i)
	{
		int prev = 0;

		for (j = 0; j < matrix_info->num_col; ++j)
		{
			if (matrix_info->matrix[i][j])
			{
				int min = INT_MAX;

				matrix_info->matrix_cn2[i][j] = 1;

				if (i - 1 >= 0)
					min = Min(min, matrix_info->matrix_cn2[i - 1][j]);
				else
					min = Min(min, 0);

				if (j - 1 >= 0)
					min = Min(min, matrix_info->matrix_cn2[i][j - 1]);
				else
					min = Min(min, 0);

				if ((i - 1 >= 0) && (j - 1 >= 0))
					min = Min(min, matrix_info->matrix_cn2[i - 1][j - 1]);

				if (min != INT_MAX)
					matrix_info->matrix_cn2[i][j] += min;

				if (matrix_info->matrix_cn2[i][j] > len)
				{
					len = matrix_info->matrix_cn2[i][j];
					bottom_right_x = i;
					bottom_right_y = j;
				}
				
				min = Min(prev, matrix_info->cn3[j]);

				if (j - 1 >= 0)
					min = Min(min, matrix_info->cn3[j - 1]);

				prev = matrix_info->cn3[j];
				matrix_info->cn3[j] = min + 1;
/*
				if (matrix_info->cn3[j] > len)
				{
					len = matrix_info->cn3[j];
					bottom_right_x = i;
					bottom_right_y = j;
				}
*/			}
			else
				matrix_info->cn3[j] = 0;

			printf(" %d", matrix_info->matrix_cn2[i][j]);
			printf("(%d)", matrix_info->cn3[j]);
		}
		printf("\n");
	}

	printf("len %d bottom right x %d y %d\n", len, bottom_right_x, bottom_right_y);
}

struct Square* MaxSquare(int **matrix, int num_row, int num_col)
{
	int len = 0;

	int max_len = Min(num_row, num_col);

	struct MatrixInfo matrix_info;
	struct Square *square = calloc(1, sizeof(struct Square));

	MatrixInfoInit(&matrix_info, matrix, num_row, num_col);
	SquareInit(square);

	printf("max_len %d\n", max_len);

	PrepareMatrixCount(&matrix_info);
	GetMaxSquare(&matrix_info);

	for (len = max_len; len > 0; --len)
	{
		if (FindMaxSquare(&matrix_info, len, square))
		{
			SquareDump(square);
			return square;
		}
	}

	printf("No square\n");

	free(square);
	return NULL;
}

int main(int argc, char *argv[])
{
#define kNumRow 4
#define kNumCol 5

	int matrix_ref[kNumRow][kNumCol] = {
		  {1, 1, 1, 0, 0}
		, {1, 1, 1, 1, 1}
		, {1, 1, 1, 1, 1}
		, {1, 0, 0, 1, 0}
	};
	
	int **matrix = calloc(kNumRow, sizeof(int*));
	int i = 0, j = 0;

	for (i = 0; i < kNumRow; ++i)
	{
		matrix[i] = calloc(kNumCol, sizeof(int));
	
		for (j = 0; j < kNumCol; ++j)
			matrix[i][j] = matrix_ref[i][j];
	}

	MaxSquare(matrix, kNumRow, kNumCol);

	return 0;
}

