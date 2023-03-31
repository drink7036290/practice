
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../lib/helpers.h"

int LongestValidParens(char *str)
{
	int index = 0, max_len = 0;
	int cn_left = 0, cn_right = 0;
	char c = '\0';

	//  cn_left_exists >= cn_right_exists

	while ((c = str[index]) != '\0')
	{
		if (c == '(')
			++cn_left;
		else
			++cn_right;

		if (cn_right > cn_left)
		{
			cn_left = 0;
			cn_right = 0;
		}
		
		if (cn_right == cn_left)
			max_len = Max(max_len, cn_right << 1);

		++index;
	}

	//  cn_left_exists <= cn_right_exists

	--index;
	cn_left = 0;
	cn_right = 0;

	while (index >= 0)
	{
		c = str[index];

		if (c == '(')
			++cn_left;
		else
			++cn_right;

		if (cn_left > cn_right)
		{
			cn_left = 0;
			cn_right = 0;
		}
		
		if (cn_right == cn_left)
			max_len = Max(max_len, cn_left << 1);

		--index;
	}

	return max_len;
}

int main(int argc, char *argv[])
{
	char *strs[] = {
		  "", ")", "(", "()", "(()"
		, "(())", "()()", "()()()", "())", ")()())"
		, "))))((()((", "(()(((()", "(()()(())(("
		, "((()()(()((()"
		, ")(()(()(((())(((((()()))((((()()(()()())())())()))()()()())(())()()(((()))))()((()))(((())()((()()())((())))(())))())((()())()()((()((())))))((()(((((()((()))(()()(())))((()))()))())"
		, "((()())()()((()((())))))"
		, ")(()))()(())()()()()(()(()))()())(()()(())()(((()()(()((()(()()((()()))(())())(()(())(()(())(()(()))(()))()()(((()())(()()(()((())))))(()(()())(()))))))())))()())()(())(((()(()))()()(()(((()))()"
	};

	int results[] = {
		  0, 0, 0, 2, 2
		, 4, 4, 6, 2, 4
		, 2, 2, 8
		, 4
		, 132
		, 24
		, 134
	};

	int i = 0, num_input = sizeof(strs) / sizeof(strs[0]);

//	num_input = 12; // [TEST]

	for (i = 0; i < num_input; ++i)
	{
		int ret = LongestValidParens(strs[i]);
		printf("%d\n", ret);

		if (ret == results[i])
			continue;

		printf("ERROR %s output %d expected %d\n", strs[i], ret, results[i]);
		break;
	}

	return 0;
}

