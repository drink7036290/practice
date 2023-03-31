
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../lib/helpers.h"

void RealToBinary(double val)
{
	int cn = 32, index = 1;
	char *str = calloc(cn + 2, sizeof(char)); // including . and \0

	str[0] = '.';

	printf("val %lf\n", val);

	while (index <= cn) // <=, including .
	{
		val *= 2;

		printf("val * 2 = %lf\n", val);

		if (val >= 1)
		{
			val -= 1;
			str[index] = '1';
		}
		else
			str[index] = '0';

		printf("val %lf str %s\n", val, str);

		if (val == 0)
			break;

		++index;
	}

	printf("str %s\n", str);

	if (index > cn)
		printf("ERROR\n");
}

int main(int argc, char *argv[])
{
	int test = 1775, invert = ~test;

	RealToBinary(atof(argv[1]));

	printf("%d 0x%x / %d 0x%x \n", test, test, invert, invert);

	return 0;
}

