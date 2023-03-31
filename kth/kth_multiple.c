
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../lib/helpers.h"

#include "../lib/heap.h"

struct ValInfo
{
	int val;
	int start_index;
};

int FuncValInfoCompare(void *obj1, void *obj2)
{
	struct ValInfo *val_info1 = obj1;
	struct ValInfo *val_info2 = obj2;

	return FuncIntCompare(&val_info1->val, &val_info2->val);
}

void ValInfoInit(struct ValInfo *val_info, int val, int start_index)
{
	val_info->val = val;
	val_info->start_index = start_index;
}

int KthMultiple(int *primes, int num_prime, int k)
{
	struct Heap heap;
	struct ValInfo *val_info = calloc(1, sizeof(struct ValInfo));

	HeapInit(&heap, kMinHeap, FuncValInfoCompare);

	ValInfoInit(val_info, 1, 0);
	HeapInsert(&heap, val_info);

	while ((val_info = HeapPop(&heap)) != NULL)
	{
		int i = 0;

		if (--k == 0)
			break;

		for (i = val_info->start_index; i < num_prime; ++i)
		{
			struct ValInfo *val_info_next = calloc(1, sizeof(struct ValInfo));
			ValInfoInit(val_info_next, val_info->val * primes[i], i);
			HeapInsert(&heap, val_info_next);
		}

		if (heap.size >= k)
			break;
	}

	while (k > 0)
	{
		val_info = HeapPop(&heap);
		--k;
	}

	return val_info->val;
}


int main(int argc, char *argv[])
{
	int primes[] = {3, 5, 7};
	int k = atoi(argv[1]);

	printf("%d\n", KthMultiple(primes, sizeof(primes)/sizeof(primes[0]), k));

	return 0;
}

