
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "lib/helpers.h"

int Rand(int start, int end)
{
	return start + (rand() % (end - start + 1));
}

void TwoDimShuffle(int **nums, int num_row, int num_col)
{
	int cn = num_row * num_col;
	int i = 0;
	
	for (i = 0; i < cn; ++i)
	{
		int j = Rand(0, i);

		Swap(&nums[i / num_col][i % num_col], &nums[j / num_col][j % num_col]);// num_col : one row
	}
}

void OneDimShuffleRecursive(int *nums, int num_size)
{
	int i = 0;

	if (num_size == 0)
		return;

	OneDimShuffleRecursive(nums, num_size - 1);

	i = Rand(0, num_size - 1);

	Swap(&nums[i], &nums[num_size - 1]);
}

void OneDimShuffle(int *nums, int num_size)
{
	int i = 0;

	for (i = 0; i < num_size; ++i)
	{
		int j = Rand(0, i);

		Swap(&nums[j], &nums[i]);
	}
}

int main(int argc, char *argv[])
{
#define kNumSize 20
	int nums[kNumSize] = {0};
#define kNumRow 3
#define kNumCol 4
	int **matrix = NULL;

	int i = 0, j = 0, total = kNumRow * kNumCol;

	for (i = 0; i < kNumSize; ++i)
		nums[i] = i;

	srand(time(NULL));

	OneDimShuffleRecursive(nums, kNumSize);

	for (i = 0; i < kNumSize; ++i)
		printf(" %d", nums[i]);
	printf("\n");

	OneDimShuffle(nums, kNumSize);

	for (i = 0; i < kNumSize; ++i)
		printf(" %d", nums[i]);
	printf("\n");

	// ==========

	matrix = calloc(kNumRow, sizeof(int*));
	for (i = 0; i < kNumRow; ++i)
		matrix[i] = calloc(kNumCol, sizeof(int));

	for (i = 0; i < kNumRow; ++i)
	{
		for (j = 0; j < kNumCol; ++j)
		{
			matrix[i][j] = total;

			if (--total == 0)
				break;
		}

		if (total == 0)
			break;
	}

	TwoDimShuffle(matrix, kNumRow, kNumCol);

	for (i = 0; i < kNumRow; ++i)
	{
		for (j = 0; j < kNumCol; ++j)
			printf(" %d", matrix[i][j]);
		printf("\n");
	}

	return 0;
}

