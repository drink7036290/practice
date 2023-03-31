
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include <limits.h>
#include <time.h>

// gcc -Wall crack_safe.c -lm

unsigned int BitMask(int index)
{
	unsigned int mask = 1;

	return (mask << (index & 0x1F));
}

bool TestBit(unsigned int *flags, int index)
{
	return (flags[index >> 5] & BitMask(index));
}

void ClearBit(unsigned int *flags, int index)
{    
	flags[index >> 5] &= (~BitMask(index));
}

void SetBit(unsigned int *flags, int index)
{    
	flags[index >> 5] |= BitMask(index);
}

struct Params
{
	char *str;
	int n;
	int k;

	int cn;
	unsigned int *bit_set;
};

void ParamsExit(struct Params *params)
{
	free(params->bit_set);
}

void ParamsInit(struct Params *params, int n, int k)
{
	int i = 0, sum = 0;

	params->cn = pow(k, n);

//	printf("cn %d\n", params->cn);

	params->str = calloc(n - 1 + params->cn + 1, sizeof(char));

	params->n = n;
	params->k = k;

	for (i = 0; i < params->n; ++i)
                sum = sum * 10 + k - 1;

	params->bit_set = calloc((sum >> 5) + 1, sizeof(unsigned int));
}

int Helper(struct Params *params, int index, int depth)
{
	int i = 0, sum = 0;

//	printf("[%zu] %s index %d depth %d\n", strlen(params->str), params->str, index, depth);

	if (depth == params->cn)
		return 1;

	for (i = index + 1; i < index + params->n; ++i)
		sum = sum * 10 + (params->str[i] - '0');

	sum *= 10;

	for (i = 0; i < params->k; ++i)
	{
		if (TestBit(params->bit_set, sum + i))
			continue;

		SetBit(params->bit_set, sum + i);

		params->str[index + params->n] = i + '0';

		if (Helper(params, index + 1, depth + 1))
			return 1;

		params->str[index + params->n] = '\0';

		ClearBit(params->bit_set, sum + i);
	}

	return 0;
}

int PrepareFirst(struct Params *params)
{
	int i = 0, sum = 0;

	if (params->str[0] == '\0')
	{
		for (i = 0; i < params->n; ++i)
			params->str[i] = '0';

		SetBit(params->bit_set, 0);
		return 1;
	}

	for (i = params->n - 1; i >= 0; --i)
	{
		int val = params->str[i] - '0';

		if (val < params->k - 1)
		{
			params->str[i] = val + 1 + '0';
			break;
		}

		params->str[i] = '0';
	}

	for (i = 0; i < params->n; ++i)
		sum = sum * 10 + params->str[i] - '0';

	SetBit(params->bit_set, sum);

	return (sum != 0);
}

char* crackSafe(int n, int k)
{
	struct Params params;

	ParamsInit(&params, n, k);

	while (PrepareFirst(&params))
	{
		printf("\n\nFirst %s\n", params.str);

		if(Helper(&params, 0, 1))
			break;
	}

	ParamsExit(&params);

	return params.str;
}

int main(int argc, char *argv[])
{
	char *str = crackSafe(atoi(argv[1]), atoi(argv[2]));

	printf("final [%zu] %s\n", strlen(str), str);

	return 0;
}

