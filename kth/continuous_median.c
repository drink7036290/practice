
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../lib/heap.h"

void HeapInsertWrapper(struct Heap *heap, int val)
{
	int *tmp = calloc(1, sizeof(int));
	*tmp = val;

	HeapInsert(heap, tmp);
}

struct Median
{
	struct Heap max_heap;
	struct Heap min_heap;
};

double MedianGet(struct Median *median)
{
	struct Heap *max_heap = &median->max_heap;
	struct Heap *min_heap = &median->min_heap;

	int *max_top = HeapTop(max_heap);

	if (max_heap->size == min_heap->size)
	{
		int *min_top = HeapTop(min_heap);

		if (min_heap->size == 0)
			return 0; // error

		

		return (*max_top + *min_top) / 2.;
	}

	return *max_top;
}

void MedianInsert(struct Median *median, int val)
{
	struct Heap *max_heap = &median->max_heap;
	struct Heap *min_heap = &median->min_heap;

	int *min_top = NULL, *max_top = NULL;

	if (max_heap->size == min_heap->size)
	{
		if (min_heap->size == 0)
			HeapInsertWrapper(max_heap, val);
		else
		{
			min_top = HeapTop(min_heap);

			if (val > *min_top)
			{
				HeapPop(min_heap);

				HeapInsertWrapper(min_heap, val);
				HeapInsertWrapper(max_heap, *min_top);
			}
			else
				HeapInsertWrapper(max_heap, val);
		}

		return;	
	}

	// max_heap->size > min_heap->size

	max_top = HeapTop(max_heap);

	if (val > *max_top)
		HeapInsertWrapper(min_heap, val);
	else
	{
		int *max_top = HeapPop(max_heap);

		HeapInsertWrapper(max_heap, val);
		HeapInsertWrapper(min_heap, *max_top);
	}
}

void MedianInsert2(struct Median *median, int val)
{
	struct Heap *max_heap = &median->max_heap;
        struct Heap *min_heap = &median->min_heap;

        int *max_top = HeapTop(max_heap);

	int *val_obj = calloc(1, sizeof(int));
	*val_obj = val;

	if (max_top == NULL)
	{
		HeapInsert(max_heap, val_obj);
		return;
	}

	if (val <= *max_top)
		HeapInsert(max_heap, val_obj);
	else
		HeapInsert(min_heap, val_obj);

	if (max_heap->size > min_heap->size + 1)
		HeapInsert(min_heap, HeapPop(max_heap));
	else if (max_heap->size < min_heap->size)
		HeapInsert(max_heap, HeapPop(min_heap));
}

void MedianDumpHeaps(struct Median *median)
{
	printf("== Max\n");
	HeapDump(&median->max_heap, FuncIntDump);

	printf("== Min\n");
	HeapDump(&median->min_heap, FuncIntDump);

	printf("== Median %lf\n", MedianGet(median));

	printf("\n");
}

void MedianInit(struct Median *median)
{
	HeapInit(&median->max_heap, kMaxHeap, FuncIntCompare);
	HeapInit(&median->min_heap, kMinHeap, FuncIntCompare);
}

void PrintContinuousMedian(int num)
{
	struct Median median, median2;
	int cn = num;

	MedianInit(&median);
	MedianInit(&median2);

	srand(time(NULL));

	while (cn > 0)
	{
		int val = rand() % 100;

		MedianInsert(&median, val);
		MedianInsert2(&median2, val);
//		MedianDumpHeaps(&median);
//		MedianDumpHeaps(&median2);

		if (MedianGet(&median) != MedianGet(&median2))
			printf("ERROR DIFF\n");

//		printf("================================ %d\n", cn);

		--cn;
	}
}

int main(int argc, char *argv[])
{
	PrintContinuousMedian(atoi(argv[1]));

	return 0;
}

