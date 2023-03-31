
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <limits.h>
#include <time.h>

#include "../lib/helpers.h"

struct Range
{
	int start;
	int end;
};

int RangeGetLen(struct Range *range)
{
	if ((range->start == -1) || (range->end == -1))
		return 0;

	return (range->end - range->start + 1);
}

void RangeDump(struct Range *range)
{
	printf("start %d end %d len %d\n", range->start, range->end, RangeGetLen(range));
}

void RangeInit(struct Range *range, int start, int end)
{
	range->start = start;
	range->end = end;
}

void Expand(char *str, int len, int left, int right, struct Range *range)
{
	while ((left >= 0) && (right < len))
	{
		if (str[left] != str[right])
			break;

		--left;
		++right;
	}

	// restore
	++left;
	--right;

	if (right - left + 1 > RangeGetLen(range))
		RangeInit(range, left, right);
}

struct Range* LongestPalindrome(char *str)
{
	int len = strlen(str);
	int index = 0;

	struct Range *range = calloc(1, sizeof(struct Range));
	RangeInit(range, -1, -1);

	while (index < len)
	{
		Expand(str, len, index, index, range);
		Expand(str, len, index, index + 1, range);

		++index;
	}

	return range;
}

int main(int argc, char *argv[])
{
	char str[] =
//		"babad";
		"cbbd";

	struct Range *range = NULL;

	if ((range = (LongestPalindrome(str))) != NULL)
		RangeDump(range);

	return 0;
}

