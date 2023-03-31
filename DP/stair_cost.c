
#include <stdio.h>
#include <stdlib.h>

#include "../lib/helpers.h"

int CostHelper(int *costs, int num_cost, int n, int *mem)
{
	if (n < 0)
		return 0;

	if (mem[n] == 0)
	{
		mem[n] = Min(CostHelper(costs, num_cost, n - 1, mem), CostHelper(costs, num_cost, n - 2, mem));

		if (n < num_cost)
			mem[n] += costs[n];

		printf("n %d cost %d\n", n, mem[n]);
	}

	return mem[n];
}

int CostClimbing(int *costs, int num_cost)
{
	int *mem = calloc(num_cost + 1, sizeof(int));

	int cost = CostHelper(costs, num_cost, num_cost, mem);

	free(mem);

	return cost;
}

int main(int argc, char *argv[])
{
//#define kNumCost 3
//	int costs[kNumCost] = {10, 15, 20};

#define kNumCost 10
        int costs[kNumCost] = {1,100,1,1,1,100,1,1,100,1};

	printf("%d\n", CostClimbing(costs, kNumCost));

	return 0;
}

