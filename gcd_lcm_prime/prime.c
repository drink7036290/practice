
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int NextPrime(int *flags, int num_flags, int prime)
{
	int i = 0;

	for (i = prime + 1; i < num_flags; ++i)
	{
		if (! flags[i])
			break;
	}
	
	printf("next %d\n", i);
	return i;
}

void DeleteNonPrimes(int *flags, int num_flags, int prime)
{
	int i = 0;

	for (i = prime * prime; i < num_flags; i += prime)
	{
		printf("prime %d i %d\n", prime, i);
		flags[i] = 1;
	}
}

void PrintPrimes(int n) // by deleting non primes
{
	int *flags = calloc((n + 1), sizeof(int)); // 0 means prime
	int prime = 2, i = 0, count = 0;

	while (prime * prime <= n)
	{
		DeleteNonPrimes(flags, n + 1, prime);
		prime = NextPrime(flags, n + 1, prime);
	}

	for (i = 2; i <= n; ++i)
	{
		if (flags[i])
			continue;

		printf(" %d", i);
		++count;
	}
	printf("\ncount %d\n", count);
}


int main(int argc, char *argv[])
{
	PrintPrimes(atoi(argv[1]));

	return 0;
}

