
#include <stdio.h>
#include <stdlib.h>

void Generate(int curr, int min, int max)
{
	if (curr > max)
		return;

	if (curr >= min)
		printf(" %d", curr);

	Generate(curr * 10, min, max);

	if (curr % 10 != 9)
		Generate(curr + 1, min, max);
}

void LexicographicalNumber(int min, int max)
{
	Generate(1, min, max);
	printf("\n");
}

int main(int argc, char *argv[])
{
	LexicographicalNumber(atoi(argv[1]), atoi(argv[2]));

	return 0;
}

