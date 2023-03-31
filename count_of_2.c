
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// gcc -Wall count_of_2.c -lm

int CountEach(int val)
{
	int ret = 0;

	while (val > 0)
	{
		if ((val % 10) == 2)
			++ret;

		val = val / 10;
	}

	return ret;
}

int CountBrute(int n)
{
	int i = 0, ret = 0;

	for (i = 0; i <= n; ++i)
		ret += CountEach(i);

	return ret;
}

/*
 *	118
 *		20-29
 *	138
 *		20-29, 120-129
 *	128
 *		20-29, 120-128
 */

int Count2sAtDigit(int num, int pos) // 1
{
	int power_of_10 = pow(10, pos); // 10
	int next_power_of_10 = power_of_10 * 10; // 100
	int right = num % power_of_10; // 8

	int round_down = num - num % next_power_of_10; // 100
	int round_up = round_down + next_power_of_10; // 200

	int digit = (num / power_of_10) % 10;

	if (digit < 2)
		return round_down / 10;

	if (digit > 2)
		return round_up / 10;

	// digit == 2
	return round_down / 10 + right + 1;
}

int Count2s(int num)
{
	int tmp = num, pos = 0, sum = 0;

	while (tmp > 0)
	{
		sum += Count2sAtDigit(num, pos);

		++pos;
		tmp /= 10;
	}

	return sum;
}

int main(int argc, char *argv[])
{
	printf("Brute %d Improved %d\n"
		, CountBrute(atoi(argv[1]))
		, Count2s(atoi(argv[1]))
	      );

	return 0;
}

