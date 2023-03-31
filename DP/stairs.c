
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/helpers.h"

#define MEMORIZATION

long long gRecursiveCount = 0;

long long CountWays(int n, int *step_lengths, int num_step_length, long long *ways)
{
	int i = 0;
	long long count = 0;

	++gRecursiveCount;

	if (n == 0)
		return 1;

	for (i = 0; i < num_step_length; ++i)
	{
		int left = 0;

		if (step_lengths[i] > n)
			continue;

		left = n - step_lengths[i];
#ifdef MEMORIZATION
		if (ways[left] == 0)
			ways[left] = CountWays(left, step_lengths, num_step_length, ways);

		count += ways[left];
#else
		count += CountWays(left, step_lengths, num_step_length, ways);
#endif
	}

//	printf("n %d count %lld\n", n, count);

	return count;
}

long long CountWaysWrapper(int n, int *step_lengths, int num_step_length)
{
	long long *ways = calloc((n + 1), sizeof(long long));
	long count = 0;

	count += CountWays(n, step_lengths, num_step_length, ways);

	free(ways);

	return count;
}

void DumpWays(long long *ways, int max_step_len)
{
	int i = 0;

	for (i = 1; i <= max_step_len; ++i)
		printf(" %lld", ways[i]);
	printf("\n");
}

long long CountWaysIterative(int n, int *step_lens, int num_step_len)
{
	int i = 0, max_step_len = 0;
	long long *ways = NULL, count = 0;

	for (i = 0; i < num_step_len; ++i)
		max_step_len = Max(max_step_len, step_lens[i]);

	// ways[K] means N-K for K >= 1
	ways = calloc(max_step_len + 1, sizeof(long long));

	for (i = 1; i <= n; ++i)
	{
		int j = 0;

		count = 0; // reset

		for (j = 0; j < num_step_len; ++j)
		{
			if (i < step_lens[j])
				continue;

			if (i == step_lens[j])
				++count;
			else
				count += ways[step_lens[j]];
		}

		// shift
		for (j = max_step_len; j > 1; --j) // Order matters !
			ways[j] = ways[j - 1];

		ways[1] = count;

//		DumpWays(ways, max_step_len);
	}

	free(ways);

	return count;
}

int main(int argc, char *argv[])
{
//#define kNumStepLength 2
//	int step_lengths[kNumStepLength] = {2, 5};

#define kNumStepLength 3
	int step_lengths[kNumStepLength] = {1, 2, 3};

	printf("recursive %lld\n", CountWaysWrapper(atoi(argv[1]), step_lengths, kNumStepLength));
//	printf("recursive calls %lld\n", gRecursiveCount);

	printf("iterative %lld\n", CountWaysIterative(atoi(argv[1]), step_lengths, kNumStepLength));

	return 0;
}

