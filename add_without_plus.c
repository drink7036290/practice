
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

int AddWithoutPlus(int a, int b)
{
	while (b > 0)
	{
		int sum = a ^ b;  // no carry
		int carry = ((a & b) << 1);

		a = sum;
		b = carry;
	}

	return a;
}

int main(int argc, char *argv[])
{
	int a = atoi(argv[1]);
	int b = atoi(argv[2]);

	printf("%d / %d\n", a + b, AddWithoutPlus(a, b));

	return 0;
}

