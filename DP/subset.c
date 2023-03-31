
#include "stdio.h"
#include "stdlib.h"

int gcn = 0;

void print_single_subset(int *rst, int *a, int cn)
{
	int i = 0;

	printf("{");

	for (i = 0; i < cn; ++i)
	{
		if (rst[i])
			printf(" %d", a[i]);
	}

	printf("}\n");

	++gcn;
}

void _print_all_subsets_r(int *rst, int i, int *a, int cn)
{
	if (i == cn)
	{
		print_single_subset(rst, a, cn);
		return;
	}

	rst[i] = 0;
		
	_print_all_subsets_r(rst, i + 1, a, cn);

	rst[i] = 1;

	_print_all_subsets_r(rst, i + 1, a, cn);
}

void print_all_subsets_r(int *a, int cn) // recursive
{
	int *rst = malloc(cn * sizeof(int));

	_print_all_subsets_r(rst, 0, a, cn);

	free(rst);
}

void print_all_subsets(int *a, int cn) // iterative
{
	int *rst = malloc(cn * sizeof(int));
	int i = 0, idx = 0;

	for (i = 0; i < cn; ++i)
		rst[i] = 0; // push

	i = cn - 1;
	while (i >= 0)
	{
//		printf("i %d val %d\n", i, rst[i]);

		if (i == cn - 1)
			print_single_subset(rst, a, cn); // pop

//		printf("i %d val %d\n", i, rst[i]);

		if (rst[i] == 1)
		{
			--i;
			continue;
		}
		
		rst[i] = 1; // push

		// reset [i+1, cn) and add i
		while (i+1 < cn)
		{
			rst[++i] = 0; // push
//			printf("i %d val %d\n", i, rst[i]);
		}

//		printf("i %d val %d\n", i, rst[i]);
	}

	free(rst);
}

#define SIZE 5

int main(int argc, char *argv[])
{
	int i = 0;
	int a[SIZE] = {1, 2, 3, 4, 5};

	for (i = 0; i < argc; ++i)
	{
		printf("%d | %s\n", i, argv[i]);
	}	

	print_all_subsets_r(a, SIZE);

	printf("cn %d\n", gcn);

	return 0;
}

