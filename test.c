
#include "stdio.h"
#include "stdlib.h"
#include "time.h"

int fn(int **test, int rows, int cols)
{
	int val = 4;
	double rst = 0;

//	printf("%d\n", test[1][2]);

	srand(time(NULL));
	rst = rand() % (val * 1000);

//	printf("rst %f /1000 %f\n", rst, rst / 1000);
}

int main(int argc, char *argv[])
{
	int **test = NULL;
	const int kRows = 2, kCols = 3;
	//int test2[kRows][kCols] = {0};

	int i = 0;

	for (i = 3; i >= -4; --i)
		printf("%d 0x%x\n", i, i);

	test = malloc(kRows * sizeof(int *));
	for (i = 0; i < kRows; ++i)
		test[i] = malloc(kCols * sizeof(int));

	fn(test, kRows, kCols);

	for (i = 0; i < kRows; ++i)
		free(test[i]);

	free(test);

	return 0;
}

