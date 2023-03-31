
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include <stdbool.h>

#include "../lib/helpers.h"

bool ParseExpression(char *str, int *index)
{
	bool ret = false;

	char op = str[*index];
/*
	if (op == '\0')
		return 
*/
//	printf("op %c index %d\n", op, *index);

	if ((op == 't') || (op == 'f'))
	{
		++*index;

		return (op == 't') ? true : false;
	}

	*index += 2; // (
	ret = ParseExpression(str, index);

	if (op == '!')
		ret = !ret;

	while (str[*index] == ',') // , or )
	{
		bool sub_ret = false;

		++*index;
		sub_ret = ParseExpression(str, index);

		if (op == '&')
			ret &= sub_ret;
		else // '|'
			ret |= sub_ret;

//		printf("next index %d val %c\n", *index, str[*index]);
	}

	if (str[*index] == ')')
		++*index;

	return ret;
}

int main(int argc, char *argv[])
{
	char str[] = "|(f)"; // 0
//	char str[] = "&(|(f))"; // 0
//	char str[] = "|(f,f,f,t)"; // 1
//	char str[] = "!(&(f,t))"; // 1
//	char str[] = "|(&(t,f,t),t)"; // 1

	int index = 0;

	printf("%d\n", ParseExpression(str, &index));

	return 0;
}

