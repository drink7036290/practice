
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../lib/helpers.h"

char* DecodeStringHelper(char **pos_pointer)
{
	char *pos = *pos_pointer;
	char *return_str = NULL;

	while (*pos != '\0')
	{
		char c = *pos;

		if ((c == ')') || (c == ']') || (c == '}'))
			break;

		if (! isdigit(c))
			return_str = StrConcatChar(return_str, c);
		else
		{
			int accu_number = 0;
			char *substring = NULL;

			while ((c = *pos) != '\0')
			{
				if (! isdigit(c))
					break;

				accu_number = accu_number * 10 + (c - '0');

				++pos;
			}

			++pos; // skip [

			substring = StrConcat(substring, DecodeStringHelper(&pos));

//			++pos; // already skipped ) ] }

			while (accu_number > 0)
			{
				return_str = StrConcat(return_str, substring);

				--accu_number;
			}
		}

		++pos;
	}

	*pos_pointer = pos;

	return return_str;
}

char* DecodeString(char *str)
{
	return DecodeStringHelper(&str);
}

int main(int argc, char *argv[])
{
//	char str[] = "2[a]";

//	char str[] = "3[a]2[bc]";
	char str[] = "3[a2(c)e]";
//	char str[] = "ef2{abc}3[cd]ef";

	printf("%s\n", DecodeString(str));

	return 0;
}

