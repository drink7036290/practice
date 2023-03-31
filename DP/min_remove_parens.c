
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../lib/helpers.h"

char* MinRemove(char *str)
{
	int len = strlen(str);

	char *tmp = calloc(len + 1, sizeof(char));
	char *tmp2 = calloc(len + 1, sizeof(char));

	char c = '\0';
	int index_from = 0, index_to = 0;

	int cn_left = 0, cn_right = 0;

	strcpy(tmp, str);

	// cn_left_exist >= cn_right_exist

	while((c = str[index_from]) != '\0')
	{
		if (c == '(')
			++cn_left;
		else if (c == ')')
			++cn_right;

		if (cn_right > cn_left)
			--cn_right;
		else
			tmp[index_to++] = c;

		++index_from;
	}

	index_from = index_to - 1;
	index_to = len - 1;

	cn_left = 0;
	cn_right = 0;

	// cn_left_exist <= cn_right_exist
	
	while (index_from >= 0)
	{
		c = tmp[index_from];

		if (c == '(')
			++cn_left;
		else if (c == ')')
			++cn_right;

		if (cn_left > cn_right)
			--cn_left;
		else
			tmp2[index_to--] = c;

		--index_from;
	}

	free(tmp);

	return tmp2 + (index_to + 1);
}

int main(int argc, char *argv[])
{
	char *strs[] = {
		  "lee(t(c)o)de)"
		, "a)b(c)d"
		, "))(("
	};

	int i = 0, num_str = sizeof(strs)/sizeof(strs[0]);

	for (i = 0; i < num_str; ++i)
		printf("%s -> %s\n", strs[i], MinRemove(strs[i]));

	return 0;
}

