
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../lib/helpers.h"

char GetLeft(char right)
{
	if (right == ')')
		return '(';

	if (right == ']')
		return '[';

	return '{';
}

int IsRight(char c)
{
	return ((c == ')') || (c == ']') || (c == '}'));
}

int IsValidParensHelper(char *str, int *index, char left)
{
	int ret = 0;
	char c = str[*index];

	printf("index %d c %c left %c\n", *index, c, left);

	if (c == '\0')
		return (left == 'x');

	if (IsRight(c))
		return (left == 'x') ? 0 : (left == GetLeft(c)) ;

	++*index;
	if ((ret = IsValidParensHelper(str, index, c)) == 0)
		return 0;

	if (left != 'x')
	{
		++*index;
		return IsValidParensHelper(str, index, left);
	}

	return 1;
}

int IsValidParens(char *str)
{
	int index = 0;

	while (str[index] != '\0')
	{
		if (! IsValidParensHelper(str, &index, 'x'))
			return 0;

		++index;
	}

	return 1;
}

int main(int argc, char *argv[])
{
//	char str[] = ")";
//	char str[] = "(";
//	char str[] = "()";
//	char str[] = "(()";
//	char str[] = "(())";
//	char str[] = "()()";

//	char str[] = "()[]{}";
	char str[] = "(]";

	printf("%d\n", IsValidParens(str));

	return 0;
}

