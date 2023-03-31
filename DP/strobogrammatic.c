
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>

// gcc -Wall strobogrammatic.c -lm


#define kNumCandidates 5

static char candidates[kNumCandidates][2] = {
	  {'0', '0'}
	, {'1', '1'}
	, {'8', '8'}
	, {'6', '9'}
	, {'9', '6'}
};

static char mapping[10];

void GenTheOther(char *str, int pos_left, int pos_right, char **strs, int *index)
{
	while (pos_left >= 0)
	{
		str[pos_right] = mapping[str[pos_left] - '0'];

		--pos_left;
		++pos_right;
	}

	strncpy(strs[*index], str, pos_right);
	++*index;
}

void GenMiddle(char *str, int pos, char **strs, int *index)
{
	int i = 0;

	for (i = 0; i < 3; ++i)
	{
		str[pos] = candidates[i][0];
		GenTheOther(str, pos - 1, pos + 1, strs, index);
	}
}

void GenStrsHelper(char *str, int pos, int cn, int n, char **strs, int *index)
{
	int i = 0;

	if (cn == 0)
	{
		if (n & 1) // odd
			GenMiddle(str, pos, strs, index);
		else
			GenTheOther(str, pos - 1, pos, strs, index);

		return;
	}	

	for (i = 0; i < kNumCandidates; ++i)
	{
		if ((pos == 0) && (candidates[i][0] == '0'))
			continue;

		str[pos] = candidates[i][0];

		GenStrsHelper(str, pos + 1, cn - 1, n, strs, index);
	}
}

void GenStrs(char **strs, int n)
{
	int index = 0, i = 0;
	char *str = calloc(n + 1, sizeof(char)); // '\0'

	for (i = 0; i < kNumCandidates ; ++i)
		mapping[candidates[i][0] - '0'] = candidates[i][1];

	GenStrsHelper(str, 0, (n >> 1), n, strs, &index);
}

char** Strobogrammatic(int n, int *num_str)
{
	char **strs = NULL;
	int i = 0;

	*num_str = 1;
	if (n >> 1)
		*num_str = 4 * pow(5, (n >> 1) - 1);

	if (n & 1) // odd
		*num_str *= 3;

	strs = calloc(*num_str, sizeof(char*));
	for (i = 0; i < *num_str; ++i)
		strs[i] = calloc(n + 1, sizeof(char)); // '\0'

	GenStrs(strs, n);

	for (i = 0; i < *num_str; ++i)
		printf(" %s", strs[i]);
	printf("\n");

	return strs;
}


int main(int argc, char *argv[])
{
	int num_str = 0;
	Strobogrammatic(atoi(argv[1]), &num_str);

	return 0;
}

