
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <limits.h>
#include <time.h>

#include "../lib/helpers.h"

int MinCoinChangeHelper(int amount, int *units, int num_unit, int *memo)             
{
	int i = 0, min = INT_MAX;

	if (amount < 0)
		return INT_MAX;
	if (amount == 0)
		return 0;

	if (memo[amount] > 0)                                                          
		return memo[amount];                                                   

	for (i = 0; i < num_unit; ++i)
	{                                           
		int cn = MinCoinChangeHelper(amount - units[i], units, num_unit, memo); 

		if (cn != INT_MAX)
			min = Min(min, 1 + cn);
	}

	memo[amount] = min;
	return min;
}               

int MinCoinChange(int amount, int *units, int num_unit)                                       
{                                                                                             
	int *memo = calloc(amount + 1, sizeof(int));

	int cn = MinCoinChangeHelper(amount, units, num_unit, memo);

	return (cn == INT_MAX) ? -1 : cn ;
}

int CoinChangeHelper(int amount, int *units, int num_unit, int index, int **memo)
{
	int unit = units[index], i = 0, cn = 0;

	if (memo[amount][index] > 0)
		return memo[amount][index];

	if (index == num_unit - 1)
		cn = (amount % unit == 0);
	else
	{
		for (i = 0; i <= amount; i += unit)
			cn += CoinChangeHelper(amount - i, units, num_unit, index + 1, memo);
	}

	memo[amount][index] = cn;
	return cn;
}

int CoinChange(int amount, int *units, int num_unit)
{
	int **memo = calloc(amount + 1, sizeof(int*));
	int i = 0;

	for (i = 0; i <= amount; ++i)
		memo[i] = calloc(num_unit, sizeof(int));

	return CoinChangeHelper(amount, units, num_unit, 0, memo);
}

int main(int argc, char *argv[])
{
	int units[] =
//		{1, 2, 5};
		{25, 10, 5, 1};

	printf("ways %d min %d\n"
		, CoinChange(atoi(argv[1]), units, sizeof(units)/sizeof(units[0]))
		, MinCoinChange(atoi(argv[1]), units, sizeof(units)/sizeof(units[0])));

	return 0;
}

