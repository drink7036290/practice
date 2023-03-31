
#include <stdio.h>
#include <stdlib.h>

// Euclidean : gcd(a, b) = gcd(b, a - b) if a > b
int gcd(int a, int b)
{
	if (b == 0)
		return a;

	return gcd(b, a % b);
}

int main(int argc, char *argv[])
{
	int a = atoi(argv[1]);
	int b = atoi(argv[2]);

	int gcd_val = gcd(a, b);

	printf("gcd %d lcm %d\n", gcd_val, a * b / gcd_val);

	return 0;
}

