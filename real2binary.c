
#include "stdio.h"
#include "stdlib.h"

void test(double num)
{
	while (num > 0)
	{
		num = num * 2;

		if (num >= 1)
		{
			printf(" 1");
			num = num - 1;
		}
		else
			printf(" 0");
	}

	printf("\n");
}

void test2(double num)
{
	double frac = 0.5;

	while (num > 0)
	{
		if (num >= frac)
		{
			printf(" 1");
			num -= frac;
		}
		else
			printf(" 0");

		frac /= 2;
	}

	printf("\n");
}

int main(int argc, char *argv[])
{
	double num = 0.125;

	test(num);
	test2(num);

	return 0;
}

