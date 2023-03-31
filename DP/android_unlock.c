
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <time.h>

static int mapping[10];
static int used[10];

int CountWaysHelper(int prev, int n/*, int *nums, int index*/)
{
	int ret = 0, i = 0;

	if (n == 0)
	{
/*		for (i = 0; i < index; ++i)
			printf(" %d", nums[i]);
		printf("\n");		
*/
		return 1;
	}

	for (i = 1; i <= 9; ++i)
	{
		if (mapping[prev] == mapping[i])
		{
			if (! used[((prev + i) >> 1)])
				continue;
		}

		if (used[i])
			continue;
		used[i] = 1;

//		nums[index] = i;

		ret += CountWaysHelper(i, n - 1/*, nums, index + 1*/);

		used[i] = 0;
	}

	return ret;
}

int CountWays(int n)
{
	int i = 0;
//	int *nums = calloc(n, sizeof(int));

	for (i = 1; i <= 9; ++i)
		used[i] = 0;

	return CountWaysHelper(0, n/*, nums, 0*/);
}

int numberOfPatterns(int m, int n){
	int ret = 0, i = 0;

	mapping[0] = 0;
	mapping[5] = 9;

	mapping[1] = 1;
	mapping[3] = 1;
	mapping[7] = 1;
	mapping[9] = 1;

	mapping[2] = 2;
	mapping[8] = 2;

	mapping[4] = 3;
	mapping[6] = 3;

	for (i = m; i <= n; ++i)
		ret += CountWays(i);

	return ret;
}

int main(int argc, char *argv[])
{
	printf("%d\n", numberOfPatterns(atoi(argv[1]), atoi(argv[2])));

	return 0;
}

