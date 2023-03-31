
#include "stdio.h"
#include "stdlib.h"

#define N 4

int T[3][N];
int idx[3];

void print(void)
{
	int i = 0, j = 0;

	for (i = 0; i < 3; ++i)
	{
//		printf("T %d\n", i);

		for (j = 0; j <= idx[i]; ++j)
			printf(" %d", T[i][j]);

		printf("\n");
	}

	printf("===================\n");
}

void MoveSingle(int T_from, int T_to)
{
//	printf("from %d idx %d / to %d idx %d\n", T_from, idx[T_from], T_to, idx[T_to]);

	T[T_to][++idx[T_to]] = T[T_from][idx[T_from]];
	T[T_from][idx[T_from]] = -1;
	--idx[T_from];

//	printf("from %d idx %d / to %d idx %d\n", T_from, idx[T_from], T_to, idx[T_to]);

	print();
}

void Move(int T_from, int T_to, int n)
{
	int T_mid = 3 - T_from - T_to;

	if (n <= 0)
		return;

	Move(T_from, T_mid, n - 1);

	MoveSingle(T_from, T_to);

	Move(T_mid, T_to, n - 1);
}

int main(int argc, char *argv[])
{
	int i = 0;

	for (i = 0; i < N; ++i)
	{
		T[0][i] = N - i;
		T[1][i] = -1;
		T[2][i] = -1;
	}

	idx[0] = N - 1;
	idx[1] = -1;
	idx[2] = -1;

	print();

	Move(0, 2, N);

	return 0;
}

