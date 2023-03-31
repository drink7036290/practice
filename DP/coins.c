
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../lib/helpers.h"

int MinCoins(int amount, int *units, int num_unit, int *map)
{
	int i = 0, min_cn = INT_MAX;

	if (amount < 0)
		return INT_MAX;

	if (amount == 0)
		return 0;

	if (map[amount] > 0)
                return map[amount];

	for (i = 0; i < num_unit; ++i)
	{
		int sub_cn = MinCoins(amount - units[i], units, num_unit, map);

		if (sub_cn != INT_MAX)
			min_cn = Min(min_cn, 1 + sub_cn);
	}

	map[amount] = min_cn;
	return min_cn;
}

int MinCoinsWrapper(int amount, int *units, int num_unit)
{
	int *map = calloc(amount + 1, sizeof(int));

	int cn = MinCoins(amount, units, num_unit, map);

	return (cn == INT_MAX) ? -1 : cn ;
}

int CountWays(int amount, int *units, int num_unit, int index, int **map)
{
	int i = 0, unit = units[index], cn = 0;

	if (map[amount][index] > 0)
		return map[amount][index];

	if (index == num_unit - 1)
	{
		map[amount][index] = (amount % unit == 0);
		return map[amount][index];
	}

	for (i = 0; i <= amount; i += unit)
		cn += CountWays(amount - i, units, num_unit, index + 1, map);

//	printf("amount %d index %d cn %d\n", amount, index, cn);

	map[amount][index] = cn;
	return cn;
}

int CountWaysWrapper(int amount, int *units, int num_unit)
{
	int **map = calloc(amount + 1, sizeof(int*));
	int i = 0;

	for (i = 0; i < amount + 1; ++i)
		map[i] = calloc(num_unit, sizeof(int));

	return CountWays(amount, units, num_unit, 0, map);
}

int main(int argc, char *argv[])
{
//#define kNumUnit 3
//	int units[kNumUnit] = {1, 2, 5};

//#define kNumUnit 1
//	int units[kNumUnit] = {1};

//#define kNumUnit 1
//	int units[kNumUnit] = {2};

#define kNumUnit 4
	int units[kNumUnit] = {25, 10, 5, 1};

//#define kNumUnit 3
//        int units[kNumUnit] = {1, 15, 25};

	printf("ways %d / min coins %d\n"
		, CountWaysWrapper(atoi(argv[1]), units, kNumUnit)
		, MinCoinsWrapper(atoi(argv[1]), units, kNumUnit));

	return 0;
}

