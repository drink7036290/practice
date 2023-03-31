
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../lib/helpers.h"

#include "../lib/hash.h"

int GetWaysHelper(char *str, int start, int end, int target, struct Hash *hash)
{
	int ret = 0, i = 0;

	char *substr = NULL;
	int len = 0, *val = NULL;

	printf("start %d end %d target %d\n", start, end, target);

	if (start > end)
	{
		printf("start %d > end %d / return 0\n", start, end);
		return 0;
	}

	if (start == end)
	{
		ret = (str[start] == (target ? '1' : '0'));
		printf("start %d == end %d / val %c / ret %d\n", start, end, str[start], ret);
		return ret;
	}

	len = end - start + 1;
	substr = calloc(len + 1 + 1, sizeof(char)); // start ~ end + target + '\0' [TRICKEY]

	for (i = 0; i < len; ++i)
		substr[i] = str[start + i];
	substr[i] = (target ? '1' : '0');

	if ((val = HashGet(hash, substr)) != NULL)
	{
		free(substr);
		return *val;
	}

	for (i = start + 1; i <= end; i += 2)
	{
		int cn_left1 = GetWaysHelper(str, start, i - 1, 1, hash);
		int cn_left0 = GetWaysHelper(str, start, i - 1, 0, hash);

		int cn_right1 = GetWaysHelper(str, i + 1, end, 1, hash);
		int cn_right0 = GetWaysHelper(str, i + 1, end, 0, hash);

		int cn_total1 = 0;

		if (str[i] == '&')
			cn_total1 = cn_left1 * cn_right1;
		else if (str[i] == '|')
			cn_total1 = cn_left1 * (cn_right1 + cn_right0) + cn_left0 * cn_right1;
		else // ^
			cn_total1 = cn_left1 * cn_right0 + cn_left0 * cn_right1;

		printf("cn_left1 %d cn_left0 %d cn_right1 %d cn_right0 %d / op %c(index %d) target %d cn_total1 %d cn_total0 %d\n"
			, cn_left1, cn_left0, cn_right1, cn_right0
			, str[i], i, target, cn_total1, (cn_left1 + cn_left0) * (cn_right1 + cn_right0) - cn_total1);

		if (target)
			ret += cn_total1;
		else
			ret += (cn_left1 + cn_left0) * (cn_right1 + cn_right0) - cn_total1;
	}

	val = calloc(1, sizeof(int));
	*val = ret;
	HashInsert(hash, substr, val);

	return ret;
}

int GetWays(char *str, int target)
{
	struct Hash hash;

	HashInit(&hash, FuncStrToInt, FuncStrCompare);

	return GetWaysHelper(str, 0, strlen(str) - 1, target, &hash);
}

int main(int argc, char *argv[])
{
//	char str[] = "";
//	int target = 0;

//	char str[] = "1";
//	int target = 1;

//	char str[] = "1^0|0|1";
//	int target = 0; // 0 or 1

	char str[] = "0&0&0&1^1|0";
	int target = 1; // 0 or 1

	printf("%d\n", GetWays(str, target));

	return 0;
}

