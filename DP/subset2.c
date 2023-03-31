
#include "stdio.h"
#include "stdlib.h"

#define SIZE 5

int a[SIZE] = { 1, 2, 3, 4, 5 };
int rst[SIZE] = { 0, 0, 0, 0, 0 };
int cn = 0;

void print_rst(int *a, int *rst)
{
	int i = 0;

	printf("[set]");

	for (i = 0; i < SIZE; ++i)
	{
		if (rst[i])
			printf(" %d", a[i]);
	}

	printf("\n");

	++cn;
}

void print_subsets_r(int *a, int idx, int *rst)
{
//	printf("idx %d\n", idx);

	if (idx >= SIZE)
	{
		print_rst(a, rst);
		return;
	}

	rst[idx] = 0;

	print_subsets_r(a, idx + 1, rst);

	rst[idx] = 1;

	print_subsets_r(a, idx + 1, rst);
}

void print_subsets(int *a)
{
	print_subsets_r(a, 0, rst);
}

void print_subsets_i(int *a, int *rst)
{
	int idx = SIZE - 1, i = 0;

	for (i = 0; i < SIZE; ++i)
		rst[i] = 0;

	cn = 0;

	while (idx >= 0)
	{
		if (idx == SIZE - 1)
			print_rst(a, rst);

		if (rst[idx] == 0)
		{
			rst[idx] = 1;

			while (idx < SIZE - 1) // trick here
				rst[++idx] = 0;

			continue;
		}

		--idx;
	}
}

int main(int argc, char *argv[])
{
	print_subsets(a);
	printf("cn %d\n", cn);

	printf("\n");

	print_subsets_i(a, rst);
	printf("cn %d\n", cn);

	return 0;
}

