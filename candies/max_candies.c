
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../lib/helpers.h"

int BinarySearch(int *piles, int num_pile, int max, long long k)
{
	int start = 1, end = max;
	int ret = 0;

	while (start <= end)
	{
		int mid = (start + end) / 2;

		long long cn = 0;
		int i = 0;

		for (i = 0; i < num_pile; ++i)
		{
			cn += piles[i] / mid;

			if (cn >= k)
				break;
		}

		if (cn >= k)
		{
			start = mid + 1;
			ret = mid;
		}
		else
			end = mid - 1;
	}

	return ret;
}

int MaxCandies(int *piles, int num_pile, long long k)
{
	int i = 0, max = INT_MIN;

	for (i = 0; i < num_pile; ++i)
		max = Max(max, piles[i]);

	return BinarySearch(piles, num_pile, max, k);
}


int main(int argc, char *argv[])
{
	int piles[] = {5, 8, 6};
	long long k = 3;

//	int piles[] = {2, 5};
//	long long k = 11;

	printf("%d\n", MaxCandies(piles, sizeof(piles)/sizeof(piles[0]), k));

	return 0;
}

