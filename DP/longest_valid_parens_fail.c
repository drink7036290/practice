
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

int LongestValidParensHelper(char *str, int *index, char left, int *r_index, int mid_len)
{
	int ret = 0, start_index = -1, len = 0;
	int sub_m_index = -1, sub_r_index = -1;
	char c = str[*index];

	printf("index %d c %c left %c\n", *index, c, left);

	if (c == '\0')
		return 0;

	if (IsRight(c))
	{
		if (left == 'x')
			return 0;

		if (left != GetLeft(c))
			return 0;

		*r_index = *index;
		return 2; // [TRICKEY!!]
	}

	++*index;
	if ((ret = LongestValidParensHelper(str, index, c, &sub_m_index, 0)) == 0)
		return 0;

	// RIGHT

	*r_index = sub_m_index; // [TRICKEY!!]

	if (left == 'x')
		return ret;

	if (str[*index] == '\0') // [TRICKEY!!]
		return ret;

	start_index = ++*index;
	printf("(right) start %d\n", start_index);
	len = LongestValidParensHelper(str, index, left, &sub_r_index, ret);

	if (len == 0)
		return ret;

//	if (sub_r_index == -1)
//		printf("ERROR len %d > 0 sub_r_index -1\n", len);

	// all )))...
	// (((( ... )))) cn_left == cn_right
	// (( ... )))) cn_left < cn_right (not exist)
	// (((( ... )) cn_left > cn_right

	printf("(right) len %d start %d end %d\n", len, start_index, sub_r_index);

	if (len >= (sub_r_index - start_index + 1)) // >=
	{
		printf("(right) return sum %d (%d + %d)\n", ret + len, ret, len);
		*r_index = sub_r_index;
		return ret + len;
	}

	printf("(right) return max %d\n", Max(ret, len));
	if (mid_len + ret >= len) // >=
	{
		*r_index = sub_m_index;
		return ret;
	}

	*r_index = sub_r_index;
	return len;
}

int LongestValidParens(char *str)
{
	int index = 0, r_index = -1;
	int max_len = 0, accu_len = 0;

	while (str[index] != '\0')
	{
		int start_index = index;
		int len = LongestValidParensHelper(str, &index, 'x', &r_index, 0);

		printf("len %d start %d end %d\n", len, start_index, index);

		if (len == (index - start_index + 1))
		{
			accu_len += len;
			max_len = Max(max_len, accu_len);
		}
		else
		{
			accu_len = len;
			max_len = Max(max_len, len);
		}

		if (str[index] == '\0') // [TRICKEY!!]
			break;

		++index;
	}

	return max_len;
}

int main(int argc, char *argv[])
{
	char *strs[] = {
		  "", ")", "(", "()", "(()"
		, "(())", "()()", "()[]{}", "(]", ")()())"
		, "))))((()((", "(()(((()", "(()()(())(("
		, "((()()(()((()"
		, ")(()(()(((())(((((()()))((((()()(()()())())())()))()()()())(())()()(((()))))()((()))(((())()((()()())((())))(())))())((()())()()((()((())))))((()(((((()((()))(()()(())))((()))()))())"
		, "((()())()()((()((())))))"
		, ")(()))()(())()()()()(()(()))()())(()()(())()(((()()(()((()(()()((()()))(())())(()(())(()(())(()(()))(()))()()(((()())(()()(()((())))))(()(()())(()))))))())))()())()(())(((()(()))()()(()(((()))()"
	};

	int results[] = {
		  0, 0, 0, 2, 2
		, 4, 4, 6, 0, 4
		, 2, 2, 8
		, 4
		, 132
		, 24
		, 134
	};

	int i = 0, num_input = sizeof(strs) / sizeof(strs[0]);

	for (i = /*0*/num_input - 1; i < num_input; ++i)
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

