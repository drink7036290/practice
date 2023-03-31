
#include "stdio.h"
#include "stdlib.h"

int multiply_i(int a, int b)
{
	int i = 1, cn = 0, left = 0, rst = a;

	while (i < b)
	{
		i <<= 1;
		++cn;
	}

	left = i - b;

	while (cn > 0)
	{
		rst <<= 1;
		--cn;
	}

	while (left > 0)
	{
		rst -= a;
		--left;
	}

	return rst;
}

int multiply_r(int a, int b)
{
	int c = 0, d = 0, ret = 0;

	if (b == 1)
		return a;

	c = b >> 1;
	d = b - (c << 1);

	ret = multiply_r(a << 1, c) + ((d) ? a : 0);

	printf("a %d b %d c %d d %d ret %d\n", a, b, c, d, ret);

	return ret;
}

int multiply(int x, int y)
{
	if ((x == 0) || (y == 0))
		return 0;

	if (x < y)
		return multiply_r(y, x);

	return multiply_r(x, y);
}

int main(int argc, char *argv[])
{
	int x = 123, y = 456;

	printf("r %d i %d (%d)\n", multiply(x, y), multiply_i(x, y), x * y);

	return 0;
}

