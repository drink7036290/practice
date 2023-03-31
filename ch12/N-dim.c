
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include "../lib/helpers.h"

void DumpMethod2(int *array, int argc, char *argv[], int index, int *shift)
{
	int i = 0, len = 0;

	if (index >= argc)
		return;

	len = atoi(argv[index]);

	if (index == argc - 1)
	{
		for (i = 0; i < len; ++i)
			printf(" %d", array[*shift + i]);
		printf("\n");

		*shift += len;
	}
	else
	{
		for (i = 0; i < len; ++i)
			DumpMethod2(array, argc, argv, index + 1, shift);
		printf("\n");
	}
}

int* Method2(int argc, char *argv[])
{
	int i = 0, cn = 1;
	int *array = NULL;

	for (i = 1; i < argc; ++i)
		cn *= atoi(argv[i]);

	array = calloc(cn, sizeof(int*));

	for (i = 0; i < cn; ++i)
		array[i] = rand() & (256 - 1);

	return array;
}

void Dump(void **array, int argc, char *argv[], int index)
{
	int i = 0, len = 0;

	if (index >= argc)
		return;

	len = atoi(argv[index]);

	if (index == argc - 1)
	{
		for (i = 0; i < len; ++i)
			printf(" %d", *((int*)array[i]));
		printf("\n");
	}
	else
	{
		for (i = 0; i < len; ++i)
			Dump(array[i], argc, argv, index + 1);
		printf("\n");
	}
}

void Assign(void **array, int argc, char *argv[], int index)
{
	int i = 0, len = 0;

	if (index >= argc)
		return;

	len = atoi(argv[index]);

	if (index == argc - 1)
	{
		for (i = 0; i < len; ++i)
		{
			int *val = array[i];
			*val = rand() & (256 - 1);
		}
	}
	else
	{
		for (i = 0; i < len; ++i)
			Assign(array[i], argc, argv, index + 1);
	}
}

void** Method1(int argc, char *argv[], int index)
{
	int i = 0, len = 0;
	void **array = NULL;

	if (index >= argc)
		return NULL;

	len = atoi(argv[index]);

	array = calloc(len, sizeof(void*));

	if (index == argc - 1)
	{
		for (i = 0; i < len; ++i)
			array[i] = calloc(1, sizeof(int));
	}
	else
	{
		for (i = 0; i < len; ++i)
			array[i] = Method1(argc, argv, index + 1);
	}

	return array;
}

int main(int argc, char *argv[])
{
	int i = 0;
	int shift = 0;

	void **array = NULL;
	int *array2 = NULL;

	if (argc < 2)
	{
		printf("ERROR argc < 2\n");
		return -1;
	}

	for (i = 1; i < argc; ++i)
		printf(" %s", argv[i]);
	printf("\n");

	srand(time(NULL));

	array = Method1(argc, argv, 1);
	Assign(array, argc, argv, 1);
//	Dump(array, argc, argv, 1);

	array2 = Method2(argc, argv);
	DumpMethod2(array2, argc, argv, 1, &shift);
	
	return 0;
}

