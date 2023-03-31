
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../lib/helpers.h"

int IsNum(char c)
{
	return ((c >= '0') && (c - '0' <= 9));
}

char *DecodeHelper(char *str, int *index)
{
	char *ret_str = calloc(1, sizeof(char)); // empty

	while (str[*index] != '\0')
	{
		char *sub_str = NULL;
		int num = 0;

		while (! IsNum(str[*index]))
		{
			char c = str[*index];

			if (c == '\0')
				return ret_str;

			if ((c == ')') || (c == ']') || (c == '}'))
				return ret_str;

			ret_str = StrConcatChar(ret_str, c);
//			printf("c %c ret_str %s\n", c, ret_str);
			++*index;
		}

		// number now until '(' '[' '{'
		while (IsNum(str[*index]))
		{
			num = num * 10 + (str[*index] - '0');
			++*index;
		}

		++*index; // '(' '[' '{'

		sub_str = DecodeHelper(str, index);

		++*index; // ')' ']' '}'

		while (num > 0)
		{
			ret_str = StrConcat(ret_str, sub_str);
			--num;
		}
	}

	return ret_str;
}

char* Decode(char *str)
{
	int index = 0;

	return DecodeHelper(str, &index);
}

int main(int argc, char *argv[])
{
//	char str[] = "2[a]";

//	char str[] = "3[a]2[bc]";
//	char str[] = "3[a2(c)e]";
	char str[] = "ef2{abc}3[cd]ef";

	printf("%s\n", Decode(str));

	return 0;
}

