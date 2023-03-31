
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include "../lib/hash.h"

// gcc -Wall -lm letter_number.c ../lib/*.c

int IsNumber(char c)
{
	return (c - '0' <= 9);
}

struct Range
{
	int start;
	int end;
};

int RangeGetLen(struct Range *range)
{
	return range->end - range->start + 1;
}

void RangeSet(struct Range *range, int start, int end)
{
	range->start = start;
	range->end = end;
}

void RangeDump(struct Range *range)
{
	printf("start %d end %d len %d\n", range->start, range->end, RangeGetLen(range));
}

void RangeInit(struct Range *range)
{
	RangeSet(range, -1, -1);
}

void RangeUpdate(struct Range *range, struct Hash *hash, int diff, int index)
{
	int *val = HashGet(hash, &diff);
	
	if (val == NULL)
	{
		int *key = calloc(1, sizeof(int));
		int *val = calloc(1, sizeof(int));

		*key = diff;
		*val = index;

		HashInsert(hash, key, val);

		return;
	}

	if ((index - *val + 1) > RangeGetLen(range))
		RangeSet(range, *val, index);
}

void FindLongest(char *str)
{
	int cn_letter = 0, cn_number = 0;
	int index = 0;
	struct Range max_range;

	char *c = str;

	struct Hash hash_diff;

	RangeInit(&max_range);
	HashInit(&hash_diff, FuncIntToInt, FuncIntCompare);

	while (*c != '\0')
	{
		if (IsNumber(*c))
			++cn_number;
		else
			++cn_letter;

		RangeUpdate(&max_range, &hash_diff, abs(cn_number - cn_letter), index);

		++index;
		++c;
	}

	RangeDump(&max_range);
}

int main(int argc, char *argv[])
{
	char str[] = "aaaa11a11aa1aa1aaaaa";

	FindLongest(str);

	return 0;
}

