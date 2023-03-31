
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../lib/helpers.h"

int PrintParensHelper(char *str, int index, int cn_left, int cn_right)
{
	int ret = 0;

	// 0 <= cn_left_residual <= cn_right_residual

	if (cn_left < 0)
		return 0;

	if (cn_left > cn_right)
		return 0;

	if (cn_right == 0)
	{
		printf("%s\n", str);
		return 1;
	}

	str[index] = '(';

	ret += PrintParensHelper(str, index + 1, cn_left - 1, cn_right);

	str[index] = ')';

	ret += PrintParensHelper(str, index + 1, cn_left, cn_right - 1);

	return ret;
}

void PrintParens(int num)
{
	char *str = calloc((num << 1) + 1, sizeof(char));

	int ret = PrintParensHelper(str, 0, num, num);

	printf("total %d\n", ret);

	free(str);
}

int main(int argc, char *argv[])
{
	PrintParens(atoi(argv[1]));

	return 0;
}

