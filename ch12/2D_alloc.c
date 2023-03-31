
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include "../lib/helpers.h"

int** Method3(int num_row, int num_col)
{
	int header = num_row * sizeof(int*);
	int data = num_row * num_col * sizeof(int);

	int **matrix = calloc(1, header + data);
	int *buf = (int*)(matrix + num_row);

	int i = 0;

	for (i = 0; i < num_row; ++i)
	{
		matrix[i] = buf;
		buf += num_col;
	}

	return matrix;
}

int* Method2(int num_row, int num_col)
{
	return calloc(num_row * num_col, sizeof(int));
}

int** Method1(int num_row, int num_col)
{
	int i = 0;

	int **matrix = calloc(num_row, sizeof(int*));

	for (i = 0; i < num_row; ++i)
		matrix[i] = calloc(num_col, sizeof(int));

	return matrix;
}

int main(int argc, char *argv[])
{
	int num_row = atoi(argv[1]), num_col = atoi(argv[2]);

	int i = 0, j = 0;
	int shift = 0;

	int **matrix1 = Method1(num_row, num_col);
	int *array2 = Method2(num_row, num_col);
	int **matrix3 = Method3(num_row, num_col);

	srand(time(NULL));

	for (i = 0; i < num_row; ++i)
	{
		for (j = 0; j < num_col; ++j)
		{
			int val = rand() % 100;

			matrix1[i][j] = val;
			array2[shift + j] = val;
			matrix3[i][j] = val;
		}

		shift += num_col;
	}

	printf("1\n");
	for (i = 0; i < num_row; ++i)
	{
		for (j = 0; j < num_col; ++j)
			printf(" %d", matrix1[i][j]);
		printf("\n");
	}
	printf("\n");

	shift = 0;

	printf("2\n");
	for (i = 0; i < num_row; ++i)
	{
		for (j = 0; j < num_col; ++j)
			printf(" %d", array2[shift + j]); 
		printf("\n");

		shift += num_col;
	}
	printf("\n");

	printf("3\n");
	for (i = 0; i < num_row; ++i)
	{
		for (j = 0; j < num_col; ++j)
			printf(" %d", matrix3[i][j]);
		printf("\n");
	}
	printf("\n");

	return 0;
}

