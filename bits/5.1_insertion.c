
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../lib/helpers.h"

/* WRONG
void TestInsert(int N, int M, int j, int i)
{
	int A = (1 << (j + 1)) - 1;
	int B = (-1) << i;
	int C = A & B;

	printf("N %x M %x\n", N, M);
	printf("A %x B %x C %x\n", A, B, C);

	N |= C;
	printf("N | C: %x\n", N);

	M <<= i;
	printf("M shift i %d: %x\n", i, M);

	N |= M;
	printf("N | M: %x\n", N);
}
*/
void TestInsert2(int N, int M, int j, int i)
{
	int A = -1 << (j + 1);
	int B = (1 << i) - 1;
	int C = A | B;

	printf("N %x M %x\n", N, M);
	printf("A %x B %x C %x\n", A, B, C);

	N &= C;
	printf("N & C: %x\n", N);

	M <<= i;
	printf("M shift i %d: %x\n", i, M);

	N |= M;
	printf("N | M: %x\n", N);
}

int main(int argc, char *argv[])
{
	int N = 1 << 10;
	int M = (1 << 4) + 3;
	int i = 2, j = 6;

//	TestInsert(N, M, j, i);
	printf("\n");
	TestInsert2(N, M, j, i);

	return 0;
}

